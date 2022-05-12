/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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
