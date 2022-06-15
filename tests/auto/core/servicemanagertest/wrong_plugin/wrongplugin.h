// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0


#ifndef WRONGPLUGIN_H
#define WRONGPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtInterfaceFramework/QIfServiceInterface>

class WrongPlugin : public QObject
{
    Q_OBJECT
    //Q_INTERFACES(QIfServiceInterface)
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "wrong_plugin.json")
public:
    explicit WrongPlugin();
    ~WrongPlugin() {}

    QStringList interfaces() const {
        return QStringList() << "wrong_plugin";
    }

    QObject *interfaceInstance(const QString &interface) const {
        Q_UNUSED(interface)
        return 0;
    }

};

#endif // WRONGPLUGIN_H
