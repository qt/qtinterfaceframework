// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFMEDIADEVICEDISCOVERYMODELBACKENDINTERFACE_H
#define QIFMEDIADEVICEDISCOVERYMODELBACKENDINTERFACE_H

#include <QtInterfaceFramework/QIfFeatureInterface>
#include <QtInterfaceFramework/QIfServiceObject>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTIFMEDIA_EXPORT QIfMediaDeviceDiscoveryModelBackendInterface : public QIfFeatureInterface
{
    Q_OBJECT
public:
    explicit QIfMediaDeviceDiscoveryModelBackendInterface(QObject *parent = nullptr);

Q_SIGNALS:

    //The ownership will be transferred to discoveryModel. This will reset the model with the new content, using add/remove is recommended
    void availableDevices(const QList<QIfServiceObject*> devices);
    void deviceAdded(QIfServiceObject *device);
    void deviceRemoved(QIfServiceObject *device);
};

#define QIfMediaDeviceDiscovery_iid "org.qt-project.interfaceframework.MediaDiscovery/1.0"

QT_END_NAMESPACE

#endif // QIFMEDIADEVICEDISCOVERYMODELBACKENDINTERFACE_H
