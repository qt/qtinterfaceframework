// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <remotemodule.h>

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    RemoteModule::registerTypes();
    RemoteModule::registerQmlTypes();

    QQmlApplicationEngine engine;
    engine.load(QUrl(u"qrc:/main.qml"_s));

    return app.exec();
}
