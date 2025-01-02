// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFMEDIADEVICE_H
#define QIFMEDIADEVICE_H

#include <QtInterfaceFramework/QIfServiceObject>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTIFMEDIA_EXPORT QIfMediaDevice : public QIfServiceObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(MediaDevice)
    QML_ANONYMOUS

    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
    explicit QIfMediaDevice(QObject *parent = nullptr);
    virtual QString type() const = 0;
    virtual QString name() const = 0;

Q_SIGNALS:
    void nameChanged(QString name);
};

class Q_QTIFMEDIA_EXPORT QIfMediaUsbDevice : public QIfMediaDevice
{
    Q_OBJECT
    QML_NAMED_ELEMENT(MediaUsbDevice)
    QML_ANONYMOUS

public:
    explicit QIfMediaUsbDevice(QObject *parent = nullptr);

    virtual QString type() const override;

    //TODO add a signal to indicate that the stick can be removed now.
public Q_SLOTS:
    virtual void eject() = 0;
};

QT_END_NAMESPACE

#endif // QIFMEDIADEVICE_H
