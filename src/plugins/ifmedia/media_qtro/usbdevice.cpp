// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "filterandbrowsemodel.h"
#include "usbdevice.h"

using namespace Qt::StringLiterals;

USBDevice::USBDevice(const QString &name, QObject *parent)
    : QIfMediaUsbDevice(parent)
    , m_name(name)
{
    m_browseModel = new FilterAndBrowseModel(this, u"QIfFilterAndBrowseModel_"_s + name);
}

QString USBDevice::name() const
{
    return m_name;
}

void USBDevice::eject()
{
    qWarning() << "Ejecting a USB Device is not supported in the simulation";
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
