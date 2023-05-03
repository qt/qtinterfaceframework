// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QCoreApplication>
#include <QDBusConnection>

#include "instrumentcluster.h"
#include "instrumentcluster_adaptor.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    InstrumentCluster cluster;
    qDBusRegisterMetaType<Warning>();
    qDBusRegisterMetaType<InstrumentClusterModule::SystemType>();

    InstrumentClusterAdaptor adaptor(&cluster);
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject(u"/"_s, &cluster);
    connection.registerService(u"Example.If.InstrumentCluster"_s);

    return app.exec();
}
