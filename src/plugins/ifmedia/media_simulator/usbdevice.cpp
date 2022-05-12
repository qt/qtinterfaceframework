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

#include "logging.h"
#include "usbbrowsebackend.h"
#include "usbdevice.h"

#include <QtInterfaceFramework/QIfFilterAndBrowseModel>

#include <QDir>

USBDevice::USBDevice(const QString &folder, QObject *parent)
    : QIfMediaUsbDevice(parent)
    , m_browseModel(new UsbBrowseBackend(folder, this))
    , m_folder(folder)
{
}

QString USBDevice::name() const
{
    return QDir(m_folder).dirName();
}

void USBDevice::eject()
{
    qCWarning(media) << "Ejecting a USB Device is not supported in the simulation";
}

QString USBDevice::folder() const
{
    return m_folder;
}

QStringList USBDevice::interfaces() const
{
    QStringList list;
    list << QStringLiteral(QIfFilterAndBrowseModel_iid);
    return list;
}

QIfFeatureInterface *USBDevice::interfaceInstance(const QString &interface) const
{
    if (interface == QStringLiteral(QIfFilterAndBrowseModel_iid))
        return m_browseModel;

    return nullptr;
}
