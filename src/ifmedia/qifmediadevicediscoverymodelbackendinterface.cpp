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

#include "qifmediadevicediscoverymodelbackendinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfMediaDeviceDiscoveryModelBackendInterface
    \inmodule QtIfMedia
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.interfaceframework.MediaDiscovery/1.0
    \brief The QIfMediaDeviceDiscoveryModelBackendInterface defines the interface for backends to the
    QIfMediaDeviceDiscoveryModel feature class.

    The QIfMediaDeviceDiscoveryModelBackendInterface is the interface used by \l QIfMediaDeviceDiscoveryModel

    The interface is discovered by a \l QIfMediaDeviceDiscoveryModel object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIfMediaDeviceDiscoveryModelBackendInterface::QIfMediaDeviceDiscoveryModelBackendInterface(QObject *parent)
    : QIfFeatureInterface(parent)
{

}

//TODO add a unit test for testing multiple features instances connecting to the same instance.

/*!
    \fn void QIfMediaDeviceDiscoveryModelBackendInterface::availableDevices(const QList<QIfServiceObject*> devices)

    This signal is emitted after a call to initialize() and indicates all the already connected \a devices, which are ready
    to be used.

    \sa initialize()
*/

/*!
    \fn void QIfMediaDeviceDiscoveryModelBackendInterface::deviceAdded(QIfServiceObject *device)

    This signal is emitted when a new \a device is added to the system and is ready to be used.
*/

/*!
    \fn void QIfMediaDeviceDiscoveryModelBackendInterface::deviceRemoved(QIfServiceObject *device)

    This signal is emitted when a \a device has been removed from the system and cannot be used anymore.
*/

QT_END_NAMESPACE
