// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "mediaindexerbackend.h"

#include <QTimer>
#include <QSettings>

Q_LOGGING_CATEGORY(qLcROQIfMediaIndexer, "interfaceframework.media.qifmediaindexerbackend.remoteobjects", QtInfoMsg)

MediaIndexerBackend::MediaIndexerBackend(QObject *parent)
    : QIfMediaIndexerControlBackendInterface(parent)
    , m_node(nullptr)
    , m_helper(new QIfRemoteObjectsReplicaHelper(qLcROQIfMediaIndexer(), this))
{
}

void MediaIndexerBackend::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized()) {
        emit progressChanged(m_replica->progress());
        emit stateChanged(m_replica->state());
        emit initializationDone();
    }

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIfMediaIndexer) << "QtIfMedia.QIfMediaIndexer wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void MediaIndexerBackend::pause()
{
    m_replica->pause();
}

void MediaIndexerBackend::resume()
{
    m_replica->resume();
}

bool MediaIndexerBackend::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcROQIfMediaIndexer) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("qtifmedia"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:qtifmedia")).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode(this);
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIfMediaIndexer) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcROQIfMediaIndexer) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIfMediaIndexerReplica>(QStringLiteral("QtIfMedia.QIfMediaIndexer")));
        setupConnections();
    }
    return true;
}

void MediaIndexerBackend::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIfRemoteObjectsReplicaHelper::onNodeError);
    connect(m_helper, &QIfRemoteObjectsReplicaHelper::errorChanged, this, &QIfFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIfRemoteObjectsReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIfFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIfMediaIndexerReplica::stateChanged, this, &MediaIndexerBackend::stateChanged);
    connect(m_replica.data(), &QIfMediaIndexerReplica::progressChanged, this, &MediaIndexerBackend::progressChanged);
}
