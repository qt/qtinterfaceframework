// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <QtIfMedia/QIfMediaDevice>

class FilterAndBrowseModel;

class USBDevice : public QIfMediaUsbDevice
{
    Q_OBJECT
public:
    explicit USBDevice(const QString &name, QObject *parent = nullptr);

    QString name() const override;
    void eject() override;

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

private:
    FilterAndBrowseModel *m_browseModel;
    QString m_name;
};

#endif // USBDEVICE_H
