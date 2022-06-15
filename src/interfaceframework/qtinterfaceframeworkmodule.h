// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QTINTERFACEFRAMEWORKMODULE_H
#define QTINTERFACEFRAMEWORKMODULE_H

#include <QtInterfaceFramework/qtifglobal.h>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QtInterfaceFrameworkModule : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(QtInterfaceFrameworkModule)
    QML_SINGLETON

public:
    explicit QtInterfaceFrameworkModule(QObject *parent = nullptr);

    enum ModelCapability {
        NoExtras = 0x0,
        SupportsGetSize = 0x1, // (the backend knows the size of the model when the query is done and the user can select a different way for loading the model content)
        SupportsFiltering = 0x2,
        SupportsSorting = 0x4,
        SupportsAndConjunction = 0x8,
        SupportsOrConjunction = 0x10,
        SupportsStatelessNavigation = 0x20, // (the backend supports to have multiple models showing different contentTypes and filters at the same time)
        SupportsInsert = 0x40,
        SupportsMove = 0x80,
        SupportsRemove = 0x100
    };
    Q_DECLARE_FLAGS(ModelCapabilities, ModelCapability)
    Q_FLAG(ModelCapabilities)

    static void registerTypes();
    static void registerQmlTypes(const QString& uri, int majorVersion = 1, int minorVersion = 0);
};

QT_END_NAMESPACE

#endif // QTINTERFACEFRAMEWORKMODULE_H
