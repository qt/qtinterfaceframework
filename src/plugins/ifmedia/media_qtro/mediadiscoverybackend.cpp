// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "mediadiscoverybackend.h"
#include "usbdevice.h"

#include <QtDebug>
#include <QTimer>
#include <QSettings>

using namespace Qt::StringLiterals;

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
    const auto it = m_serviceSettings.constFind(u"MediaDiscoveryModel"_s);

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
            qCInfo(qLcROQIfMediaDiscovery) << "SERVER_CONF_PATH environment variable is set.\n"
                                         << "Overriding service setting: 'MediaDiscoveryModel.connectionUrl'";

            qCInfo(qLcROQIfMediaDiscovery) << "Using SERVER_CONF_PATH is deprecated and will be removed"
                                            "in future Qt versions.";
            qCInfo(qLcROQIfMediaDiscovery) << "Please use the new InterfaceFrameworkConfiguration item"
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
        url = u"local:qtifmedia"_s;

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
        m_replica.reset(m_node->acquire<QIfMediaDiscoveryModelReplica>(u"QtIfMedia.QIfMediaDiscoveryModel"_s));
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
