// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <QtIfMedia/QIfMediaDevice>

class UsbBrowseBackend;

class USBDevice : public QIfMediaUsbDevice
{
    Q_OBJECT
public:
    explicit USBDevice(const QString &folder, QObject *parent = nullptr);

    QString name() const override;
    void eject() override;

    QString folder() const;

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

private:
    UsbBrowseBackend *m_browseModel;
    QString m_folder;
};

#endif // USBDEVICE_H
