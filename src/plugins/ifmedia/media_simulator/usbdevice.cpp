// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
