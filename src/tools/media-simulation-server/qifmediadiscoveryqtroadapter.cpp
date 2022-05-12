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

#include "qifmediadiscoveryqtroadapter.h"
#include "qiffilterandbrowsemodelqtroadapter.h"
#include "core.h"

#include <QIfMediaDevice>
#include <QRemoteObjectRegistryHost>

QIfMediaDiscoveryModelQtRoAdapter::QIfMediaDiscoveryModelQtRoAdapter(MediaDiscoveryBackend *parent)
    : QIfMediaDiscoveryModelQtRoAdapter(QStringLiteral("QtIfMedia.QIfMediaDiscoveryModel"), parent)
{
}

QIfMediaDiscoveryModelQtRoAdapter::QIfMediaDiscoveryModelQtRoAdapter(const QString &remoteObjectsLookupName, MediaDiscoveryBackend *parent)
    : QIfMediaDiscoveryModelSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
{
    QMap<QString, QIfServiceObject*> deviceMap = m_backend->deviceMap();

    for (auto it = deviceMap.cbegin(); it != deviceMap.cend(); it++) {
        QIfMediaDevice *mediaDevice = qobject_cast<QIfMediaDevice *>(it.value());
        if (mediaDevice)
            createDeviceAdapter(mediaDevice);
    }

    connect(m_backend, &MediaDiscoveryBackend::deviceAdded, this, &QIfMediaDiscoveryModelQtRoAdapter::onDeviceAdded);
    connect(m_backend, &MediaDiscoveryBackend::deviceRemoved, this, &QIfMediaDiscoveryModelQtRoAdapter::onDeviceRemoved);
}


QString QIfMediaDiscoveryModelQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

QStringList QIfMediaDiscoveryModelQtRoAdapter::devices() const
{
    return QStringList(m_hostMap.keys());
}

void QIfMediaDiscoveryModelQtRoAdapter::onDeviceAdded(QIfServiceObject *device)
{
    QIfMediaDevice *mediaDevice = qobject_cast<QIfMediaDevice *>(device);
    if (!mediaDevice)
        return;

    createDeviceAdapter(mediaDevice);
    emit deviceAdded(mediaDevice->name());
}

void QIfMediaDiscoveryModelQtRoAdapter::onDeviceRemoved(QIfServiceObject *device)
{
    QIfMediaDevice *mediaDevice = qobject_cast<QIfMediaDevice *>(device);
    if (!mediaDevice)
        return;

    emit deviceRemoved(mediaDevice->name());
    auto instance =  m_hostMap.take(mediaDevice->name());

    qDebug() << "Removing USB Instance" << mediaDevice->name();
    Core::instance()->host()->disableRemoting(instance);
    delete instance;
}

void QIfMediaDiscoveryModelQtRoAdapter::createDeviceAdapter(QIfMediaDevice *device)
{
    qDebug() << "Adding USB Instance" << device->name();

    QIfFilterAndBrowseModelInterface *searchAndBrowseBackend = qif_interface_cast<QIfFilterAndBrowseModelInterface *>(device->interfaceInstance(QStringLiteral(QIfFilterAndBrowseModel_iid)));

    searchAndBrowseBackend->initialize();
    auto instance = new QIfFilterAndBrowseModelQtRoAdapter(QStringLiteral("QIfFilterAndBrowseModel_") + device->name(), searchAndBrowseBackend);
    Core::instance()->host()->enableRemoting<QIfFilterAndBrowseModelAddressWrapper>(instance);

    m_hostMap.insert(device->name(), instance);
}
