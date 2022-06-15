// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0


#ifndef SIMPLESTATICPLUGIN_H
#define SIMPLESTATICPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtInterfaceFramework/QIfServiceInterface>

class SimpleStaticPlugin : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIfServiceInterface)
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "simple_plugin.json")
public:
    explicit SimpleStaticPlugin();
    ~SimpleStaticPlugin() {}

    QStringList interfaces() const override {
        return QStringList() << "simple_plugin_static";
    }

    QIfFeatureInterface *interfaceInstance(const QString &interface) const override {
        Q_UNUSED(interface)
        return 0;
    }

};

#endif // SIMPLESTATICPLUGIN_H
