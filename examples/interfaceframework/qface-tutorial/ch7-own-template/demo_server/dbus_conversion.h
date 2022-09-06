// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef DBUS_CONVERSION_H
#define DBUS_CONVERSION_H

#include <QDBusArgument>
#include "warning.h"
#include "instrumentclustermodule.h"

QT_BEGIN_NAMESPACE

Q_ALWAYS_INLINE QDBusArgument &operator<<(QDBusArgument &argument, const Warning &data)
{
    argument.beginStructure();
    argument << data.color();
    argument << data.text();
    argument << data.icon();
    argument.endStructure();

    return argument;
}

Q_ALWAYS_INLINE const QDBusArgument &operator>>(const QDBusArgument &argument, Warning &data)
{
    argument.beginStructure();
    QString color;
    QString text;
    QString icon;
    argument >> color;
    argument >> text;
    argument >> icon;
    data.setColor(color);
    data.setText(text);
    data.setIcon(icon);
    argument.endStructure();

    return argument;
}

Q_ALWAYS_INLINE QDBusArgument &operator<<(QDBusArgument &argument, InstrumentClusterModule::SystemType data)
{
    argument.beginStructure();
    argument << quint32(data);
    argument.endStructure();

    return argument;
}

Q_ALWAYS_INLINE const QDBusArgument &operator>>(const QDBusArgument &argument, InstrumentClusterModule::SystemType &data)
{
    argument.beginStructure();
    quint32 systemType;
    argument >> systemType;
    data = InstrumentClusterModule::toSystemType(systemType, nullptr);
    argument.endStructure();

    return argument;
}

QT_END_NAMESPACE

#endif // DBUS_CONVERSION_H
