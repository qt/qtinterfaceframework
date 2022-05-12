/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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
