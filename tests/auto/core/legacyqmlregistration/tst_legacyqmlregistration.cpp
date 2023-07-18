// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtTest>
#include <QtInterfaceFrameworkModule>
#include <QQmlEngine>
#include <QQmlComponent>

class tst_QMLRegistration : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testRegistration_data();
    void testRegistration();
};

void tst_QMLRegistration::testRegistration_data()
{
    QTest::addColumn<QByteArray>("type");
    QTest::addColumn<QString>("error");
    QTest::newRow("AbstractFeatureListModel") << QByteArray("AbstractFeatureListModel{}") << ":1 AbstractFeatureListModel is not accessible directly";
    QTest::newRow("AbstractFeature") << QByteArray("AbstractFeature{}") << ":1 AbstractFeature is not accessible directly";
    QTest::newRow("AbstractZonedFeature") << QByteArray("AbstractZonedFeature{}") << ":1 AbstractZonedFeature is not accessible directly";
    QTest::newRow("PagingModel") << QByteArray("PagingModel{}") << QString();
    QTest::newRow("FilterAndBrowseModel") << QByteArray("FilterAndBrowseModel{}") << QString();
    QTest::newRow("InterfaceFrameworkConfiguration") << QByteArray("InterfaceFrameworkConfiguration{}") << QString();
    QTest::newRow("ServiceManager") << QByteArray("ListView { model: ServiceManager } ") << QString();
    QTest::newRow("QtInterfaceFrameworkModule") << QByteArray("QtObject { property int count: QtInterfaceFrameworkModule.NoExtras }") << QString();
}

void tst_QMLRegistration::testRegistration()
{
    QFETCH(QByteArray, type);
    QFETCH(QString, error);

    QtInterfaceFrameworkModule::registerQmlTypes("MyTypes", 1, 0);
    QQmlEngine engine;

    QByteArray qml ("import QtQuick; import MyTypes; " + type);

    QQmlComponent component(&engine);
    component.setData(qml, QUrl());
    std::unique_ptr<QObject> obj(component.create());
    if (error.isEmpty())
        QVERIFY2(obj, qPrintable(component.errorString()));
    else
        QCOMPARE(component.errorString().trimmed(), error);
}

QTEST_MAIN(tst_QMLRegistration)

#include "tst_legacyqmlregistration.moc"
