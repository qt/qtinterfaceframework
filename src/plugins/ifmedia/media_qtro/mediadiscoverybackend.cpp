/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

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

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIfMediaDiscovery) << "QtIfMedia.QIfMediaDiscoveryModel wasn't initialized within the timeout period. Please make sure the server is running.";
    });
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

bool MediaDiscoveryBackend::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcROQIfMediaDiscovery) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
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
            qCCritical(qLcROQIfMediaDiscovery) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcROQIfMediaDiscovery) << "Connecting to" << m_url;
             m_replica.reset(m_node->acquire<QIfMediaDiscoveryModelReplica>(QStringLiteral("QtIfMedia.QIfMediaDiscoveryModel")));
        setupConnections();
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
