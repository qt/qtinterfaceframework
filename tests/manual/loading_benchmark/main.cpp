// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "qloggingcategory.h"
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include <benchmarkfactory.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    BenchmarkFactory::registerQmlTypes();

    QLoggingCategory::setFilterRules("qt.if.servicemanagement.perf*=true");

    const QUrl url(u"qrc:/qt/qml/benchmark/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
