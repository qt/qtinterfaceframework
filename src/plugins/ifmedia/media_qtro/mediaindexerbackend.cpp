// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "mediaindexerbackend.h"

#include <QTimer>
#include <QSettings>

using namespace Qt::StringLiterals;

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
}

void MediaIndexerBackend::pause()
{
    m_replica->pause();
}

void MediaIndexerBackend::resume()
{
    m_replica->resume();
}

void MediaIndexerBackend::updateServiceSettings(const QVariantMap &settings)
{
    if (m_serviceSettings == settings)
        return;

    m_serviceSettings = settings;
    connectToNode();
}

bool MediaIndexerBackend::connectToNode()
{
    QUrl url;
    const auto it = m_serviceSettings.constFind(u"MediaIndexer"_s);

    if (it != m_serviceSettings.constEnd())
        url = it->toMap().value(u"connectionUrl"_s).toUrl();
    if (url.isEmpty())
        url = m_serviceSettings.value(u"connectionUrl"_s).toUrl();

    static QString configPath;
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(u"qtifmedia"_s);
        url = QUrl(settings.value(u"Registry"_s).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcROQIfMediaIndexer) << "SERVER_CONF_PATH environment variable is set.\n"
                                         << "Overriding service setting: 'MediaIndexer.connectionUrl'";

            qCInfo(qLcROQIfMediaIndexer) << "Using SERVER_CONF_PATH is deprecated and will be removed"
                                            "in future Qt versions.";
            qCInfo(qLcROQIfMediaIndexer) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty() && QFile::exists(u"./server.conf"_s)) {
        configPath = u"./server.conf"_s;

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(u"qtifmedia"_s);
        url = QUrl(settings.value(u"Registry"_s).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcROQIfMediaIndexer) << "Reading url from ./server.conf.\n"
                                         << "Overriding service setting: 'MediaIndexer.connectionUrl'";

            qCInfo(qLcROQIfMediaIndexer) << "Using ./server.conf. is deprecated and will be removed"
                                            "in future Qt versions.";
            qCInfo(qLcROQIfMediaIndexer) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty())
        url = u"local:qtifmedia"_s;

    if (m_url != url) {
        // QtRO doesn't allow to change the URL without destroying the Node
        if (m_node) {
            qCInfo(qLcROQIfMediaIndexer) << "Disconnecting from" << m_url;
            delete m_node;
        }

        m_url = url;

        m_node = new QRemoteObjectNode(this);
        qCInfo(qLcROQIfMediaIndexer) << "Connecting to" << m_url;
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIfMediaIndexer) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        m_replica.reset(m_node->acquire<QIfMediaIndexerReplica>(u"QtIfMedia.QIfMediaIndexer"_s));
        setupConnections();

        const int defaultTimeout = 3000;
        int connectionTimeout = defaultTimeout;
        if (it != m_serviceSettings.constEnd())
            connectionTimeout = it->toMap().value(u"connectionTimeout"_s, defaultTimeout).toInt();

        if (connectionTimeout == defaultTimeout)
            connectionTimeout = m_serviceSettings.value(u"connectionTimeout"_s, defaultTimeout).toInt();

        if (connectionTimeout != -1) {
            QTimer::singleShot(connectionTimeout, this, [this](){
                if (!m_replica->isInitialized())
                    qCWarning(qLcROQIfMediaIndexer) << "QtIfMedia.QIfMediaIndexer wasn't initialized within the timeout period. Please make sure the server is running.";
            });
        }
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

#include "moc_mediaindexerbackend.cpp"
