// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifmediadiscoveryqtroadapter.h"
#include "qiffilterandbrowsemodelqtroadapter.h"
#include "core.h"

#include <QIfMediaDevice>
#include <QRemoteObjectRegistryHost>

using namespace Qt::StringLiterals;

QIfMediaDiscoveryModelQtRoAdapter::QIfMediaDiscoveryModelQtRoAdapter(MediaDiscoveryBackend *parent)
    : QIfMediaDiscoveryModelQtRoAdapter(u"QtIfMedia.QIfMediaDiscoveryModel"_s, parent)
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
    auto instance = new QIfFilterAndBrowseModelQtRoAdapter(u"QIfFilterAndBrowseModel_"_s + device->name(), searchAndBrowseBackend);
    Core::instance()->host()->enableRemoting<QIfFilterAndBrowseModelAddressWrapper>(instance);

    m_hostMap.insert(device->name(), instance);
}
