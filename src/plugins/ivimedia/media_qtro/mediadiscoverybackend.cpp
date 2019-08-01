/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "mediadiscoverybackend.h"
#include "usbdevice.h"

#include <QtDebug>
#include <QTimer>
#include <QSettings>

Q_LOGGING_CATEGORY(qLcROQIviMediaDiscovery, "qtivi.media.qivimediadiscoverybackend.remoteobjects", QtInfoMsg)

MediaDiscoveryBackend::MediaDiscoveryBackend(QObject *parent)
    : QIviMediaDeviceDiscoveryModelBackendInterface(parent)
    , m_node(nullptr)
    , m_initialized(false)
    , m_helper(new QIviRemoteObjectReplicaHelper(qLcROQIviMediaDiscovery(), this))
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
            qCCritical(qLcROQIviMediaDiscovery) << "QtIviMedia.QIviMediaDiscoveryModel wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void MediaDiscoveryBackend::onDevicesChanged(const QStringList &devices)
{
    if (m_initialized)
        return;

    for (const QString &device : devices) {
        qCInfo(qLcROQIviMediaDiscovery) << "Adding USB Device: " << device;
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
    QIviServiceObject *dev = m_deviceMap.take(device);
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
            qCInfo(qLcROQIviMediaDiscovery) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("qtivimedia"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:qtivimedia")).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode(this);
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIviMediaDiscovery) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcROQIviMediaDiscovery) << "Connecting to" << m_url;
             m_replica.reset(m_node->acquire<QIviMediaDiscoveryModelReplica>(QStringLiteral("QtIviMedia.QIviMediaDiscoveryModel")));
        setupConnections();
    }
    return true;
}

void MediaDiscoveryBackend::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIviRemoteObjectReplicaHelper::onNodeError);
    connect(m_helper, &QIviRemoteObjectReplicaHelper::errorChanged, this, &QIviFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIviRemoteObjectReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIviMediaDiscoveryModelReplica::devicesChanged, this, &MediaDiscoveryBackend::onDevicesChanged);
    connect(m_replica.data(), &QIviMediaDiscoveryModelReplica::deviceAdded, this, &MediaDiscoveryBackend::onDeviceAdded);
    connect(m_replica.data(), &QIviMediaDiscoveryModelReplica::deviceRemoved, this, &MediaDiscoveryBackend::onDeviceRemoved);
}
