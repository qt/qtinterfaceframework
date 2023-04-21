// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "mediadiscoverybackend.h"
#include "usbdevice.h"

#include <QtDebug>
#include <QTimer>
#include <QSettings>

Q_LOGGING_CATEGORY(qLcROQIfMediaDiscovery, "interfaceframework.media.qifmediadiscoverybackend.remoteobjects", QtInfoMsg)

MediaDiscoveryBackend::MediaDiscoveryBackend(QObject *parent)
    : QIfMediaDeviceDiscoveryModelBackendInterface(parent)
    , m_node(nullptr)
    , m_initialized(false)
    , m_helper(new QIfRemoteObjectsReplicaHelper(qLcROQIfMediaDiscovery(), this))
{
}

void MediaDiscoveryBackend::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized()) {
        onDevicesChanged(m_replica->devices());
        emit initializationDone();
    }
}

void MediaDiscoveryBackend::onDevicesChanged(const QStringList &devices)
{
    if (m_initialized)
        return;

    for (const QString &device : devices) {
        qCInfo(qLcROQIfMediaDiscovery) << "Adding USB Device: " << device;
        m_deviceMap.insert(device, new USBDevice(device));
    }
    emit availableDevices(m_deviceMap.values());
    m_initialized = true;
}

void MediaDiscoveryBackend::onDeviceAdded(const QString &device)
{
    USBDevice *dev = new USBDevice(device);
    m_deviceMap.insert(device, dev);
    emit deviceAdded(dev);
}

void MediaDiscoveryBackend::onDeviceRemoved(const QString &device)
{
    QIfServiceObject *dev = m_deviceMap.take(device);
    emit deviceRemoved(dev);
}

void MediaDiscoveryBackend::updateServiceSettings(const QVariantMap &settings)
{
    if (m_serviceSettings == settings)
        return;

    m_serviceSettings = settings;
    connectToNode();
}

bool MediaDiscoveryBackend::connectToNode()
{
    QUrl url;
    const auto it = m_serviceSettings.constFind(QStringLiteral("MediaDiscoveryModel"));

    if (it != m_serviceSettings.constEnd())
        url = it->toMap().value(QStringLiteral("connectionUrl")).toUrl();
    if (url.isEmpty())
        url = m_serviceSettings.value(QStringLiteral("connectionUrl")).toUrl();

    static QString configPath;
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(QStringLiteral("qtifmedia"));
        url = QUrl(settings.value(QStringLiteral("Registry")).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcROQIfMediaDiscovery) << "SERVER_CONF_PATH environment variable is set.\n"
                                         << "Overriding service setting: 'MediaDiscoveryModel.connectionUrl'";

            qCInfo(qLcROQIfMediaDiscovery) << "Using SERVER_CONF_PATH is deprecated and will be removed"
                                            "in future Qt versions.";
            qCInfo(qLcROQIfMediaDiscovery) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty() && QFile::exists(QStringLiteral("./server.conf"))) {
        configPath = QStringLiteral("./server.conf");

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(QStringLiteral("qtifmedia"));
        url = QUrl(settings.value(QStringLiteral("Registry")).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcROQIfMediaDiscovery) << "Reading url from ./server.conf.\n"
                                         << "Overriding service setting: 'MediaDiscoveryModel.connectionUrl'";

            qCInfo(qLcROQIfMediaDiscovery) << "Using ./server.conf. is deprecated and will be removed"
                                            "in future Qt versions.";
            qCInfo(qLcROQIfMediaDiscovery) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty())
        url = QStringLiteral("local:qtifmedia");

    if (m_url != url) {
        // QtRO doesn't allow to change the URL without destroying the Node
        if (m_node) {
            qCInfo(qLcROQIfMediaDiscovery) << "Disconnecting from" << m_url;
            delete m_node;
        }

        m_url = url;

        m_node = new QRemoteObjectNode(this);
        qCInfo(qLcROQIfMediaDiscovery) << "Connecting to" << m_url;
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIfMediaDiscovery) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        m_replica.reset(m_node->acquire<QIfMediaDiscoveryModelReplica>(QStringLiteral("QtIfMedia.QIfMediaDiscoveryModel")));
        setupConnections();

        const int defaultTimeout = 3000;
        int connectionTimeout = defaultTimeout;
        if (it != m_serviceSettings.constEnd())
            connectionTimeout = it->toMap().value(QStringLiteral("connectionTimeout"), defaultTimeout).toInt();

        if (connectionTimeout == defaultTimeout)
            connectionTimeout = m_serviceSettings.value(QStringLiteral("connectionTimeout"), defaultTimeout).toInt();

        if (connectionTimeout != -1) {
            QTimer::singleShot(connectionTimeout, this, [this](){
                if (!m_replica->isInitialized())
                    qCWarning(qLcROQIfMediaDiscovery) << "QtIfMedia.QIfMediaDiscoveryModel wasn't initialized within the timeout period. Please make sure the server is running.";
            });
        }
    }
    return true;
}

void MediaDiscoveryBackend::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIfRemoteObjectsReplicaHelper::onNodeError);
    connect(m_helper, &QIfRemoteObjectsReplicaHelper::errorChanged, this, &QIfFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIfRemoteObjectsReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIfFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIfMediaDiscoveryModelReplica::devicesChanged, this, &MediaDiscoveryBackend::onDevicesChanged);
    connect(m_replica.data(), &QIfMediaDiscoveryModelReplica::deviceAdded, this, &MediaDiscoveryBackend::onDeviceAdded);
    connect(m_replica.data(), &QIfMediaDiscoveryModelReplica::deviceRemoved, this, &MediaDiscoveryBackend::onDeviceRemoved);
}
