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

#ifndef QIFMEDIADEVICE_H
#define QIFMEDIADEVICE_H

#include <QtInterfaceFramework/QIfServiceObject>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTIFMEDIA_EXPORT QIfMediaDevice : public QIfServiceObject
{
    Q_OBJECT

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

public:
    explicit QIfMediaUsbDevice(QObject *parent = nullptr);

    virtual QString type() const override;

    //TODO add a signal to indicate that the stick can be removed now.
public Q_SLOTS:
    virtual void eject() = 0;
};

QT_END_NAMESPACE

#endif // QIFMEDIADEVICE_H
