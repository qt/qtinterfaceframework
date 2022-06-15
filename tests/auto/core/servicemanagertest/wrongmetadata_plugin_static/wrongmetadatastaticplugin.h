// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0


#ifndef WRONGMETADATASTATICPLUGIN_H
#define WRONGMETADATASTATICPLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtInterfaceFramework/QIfServiceInterface>

class WrongMetadataStaticPlugin : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIfServiceInterface)
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "wrongmetadata.json")
public:
    explicit WrongMetadataStaticPlugin();
    ~WrongMetadataStaticPlugin() {}

    QStringList interfaces() const override {
        return QStringList() << "wrongmetadata_static";
    }

    QIfFeatureInterface *interfaceInstance(const QString &interface) const override {
        Q_UNUSED(interface)
        return 0;
    }

};

#endif // WRONGMETADATASTATICPLUGIN_H
