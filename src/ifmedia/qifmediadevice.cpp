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

#include "qifmediadevice.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfMediaDevice
    \inmodule QtIfMedia
    \brief QIfMediaDevice is the base class for all media devices.

    A Media Device is an externally or internally connected device on which media is stored.
    It can be a physical medium like a USB thumbdrive or a CD/DVD, but it could also be a DLNA Media Server running
    on a different machine e.g. on the rear seat entertainment system.

    Media Devices can be retrieved by using the QIfMediaDeviceDiscoveryModel, which asks the backend
    which devices it knows of that are currently connected to the Interface Framework system.

    \section1 Supported Media Devices
    \target SupportedMediaDevices

    The following media devices are supported by the QIfMedia module:
    \annotatedlist qtif_mediadevices
*/

/*!
    \qmltype MediaDevice
    \instantiates QIfMediaDevice
    \inqmlmodule QtInterfaceFramework.Media
    \inherits ServiceObject
    \brief MediaDevice is the base item for all Media Devices.

    A Media Device is an externally or internally connected device on which media is stored.
    It can be a physical medium like a USB thumbdrive or a CD/DVD, but it could also be a DLNA Media Server running
    on a different machine e.g. on the rear seat entertainment system.

    Media Devices can be retrieved by using the MediaDeviceDiscoveryModel, which asks the backend
    which devices it knows of that are currently connected to the Interface Framework system.

    \section1 Supported Media Devices
    \target QmlSupportedMediaDevices

    The following media devices are supported by the QIfMedia module:
    \annotatedlist qtif_qmlmediadevices
*/

/*!
    \qmlproperty string MediaDevice::name
    The name of the media device. E.g. the CD-name or the name of the thumbdrive.
*/

/*!
    \property QIfMediaDevice::name
    The name of the media device. E.g. the CD-name or the name of the thumbdrive.
*/

/*!
    \qmlproperty string MediaDevice::type
    The type of the media device. E.g. "usb", "CD". See \l QmlSupportedMediaDevices for the supported Devices.
*/

/*!
    \property QIfMediaDevice::type
    The type of the media device. E.g. "usb", "CD". See \l SupportedMediaDevices for the supported Devices.
*/

QIfMediaDevice::QIfMediaDevice(QObject *parent)
    : QIfServiceObject(parent)
{

}

/*!
    \class QIfMediaUsbDevice
    \inmodule QtIfMedia
    \ingroup qtif_mediadevices
    \brief QIfMediaUsbDevice represents a USB device connected to the system.

    This device instance can be used as a serviceObject for other Features e.g. the
    QIfFilterAndBrowseModel to browse through the content. Whether a feature is supported
    by this device is decided by the backend implementing it.

    \note The type of this device is \e "usb"
*/

/*!
    \qmltype MediaUsbDevice
    \instantiates QIfMediaUsbDevice
    \inqmlmodule QtInterfaceFramework.Media
    \ingroup qtif_qmlmediadevices
    \inherits MediaDevice
    \brief MediaUsbDevice represents a USB device connected to the system.

    This device instance can be used as a serviceObject for other Features e.g. the
    FilterAndBrowseModel to browse through the content. Whether a feature is supported
    by this device is decided by the backend implementing it.

    \note The type of this device is \e "usb"
*/

QIfMediaUsbDevice::QIfMediaUsbDevice(QObject *parent)
    : QIfMediaDevice(parent)
{

}

/*!
    \reimp

    Returns \e "usb".
*/
QString QIfMediaUsbDevice::type() const
{
    return QLatin1String("usb");
}

/*!
    \qmlmethod MediaUsbDevice::eject()

    Ejects the USB media device and makes sure all data is written to the thumb-drive so it can safely be removed.
*/

/*!
    \fn void QIfMediaUsbDevice::eject()

    Ejects the USB media device and makes sure all data is written to the thumb-drive so it can safely be removed.
*/

QT_END_NAMESPACE
