// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtTest>
#include <QSignalSpy>
#include <QQmlEngine>
#include <QIfServiceManager>
#include <QIfAbstractFeature>
#include <QIfConfiguration>

#include "includetester.h"
#include "test.h"
#include "common.h"

using namespace Qt::StringLiterals;

class CrossModuleSimulationTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();
    void testCrossModuleEnumInit();
    void testCrossModuleEnumPropertyChange();
};

void CrossModuleSimulationTest::initTestCase()
{
    include::test::Test::registerTypes();
    QVERIFY(QIfConfiguration::setDiscoveryMode(
        u"include.test"_s, QIfAbstractFeature::LoadOnlySimulationBackends));
}

void CrossModuleSimulationTest::cleanup()
{
    QIfServiceManager::instance()->unloadAllBackends();
}

/*!
    Verify that a simulation backend with cross-module enum properties
    initializes without crashing.
*/
void CrossModuleSimulationTest::testCrossModuleEnumInit()
{
    include::test::IncludeTester tester;
    QSignalSpy initSpy(&tester, &QIfAbstractFeature::isInitializedChanged);
    QVERIFY(initSpy.isValid());
    tester.startAutoDiscovery();

    if (!tester.isInitialized())
        QVERIFY(initSpy.wait(5000));

    QVERIFY(tester.isInitialized());

    QVERIFY(qmlTypeId("include.test.simulation", 1, 0, "IncludeTesterBackend") >= 0);
    QVERIFY(qmlTypeId("include.test.simulation", 1, 0, "NestedStructModelModelBackend") >= 0);
}

/*!
    Verify that setting a cross-module enum property through the simulation
    backend does not cause infinite recursion in the QML/C++ call cycle.
*/
void CrossModuleSimulationTest::testCrossModuleEnumPropertyChange()
{
    include::test::IncludeTester tester;
    QSignalSpy initSpy(&tester, &QIfAbstractFeature::isInitializedChanged);
    QVERIFY(initSpy.isValid());
    tester.startAutoDiscovery();

    if (!tester.isInitialized())
        QVERIFY(initSpy.wait(5000));
    QVERIFY(tester.isInitialized());

    QSignalSpy enumSpy(&tester, &include::test::IncludeTester::commonEnumChanged);
    QVERIFY(enumSpy.isValid());

    tester.setCommonEnum(Common::Common::CommonValue2);

    if (enumSpy.size() == 0)
        QVERIFY(enumSpy.wait(5000));

    QCOMPARE(tester.commonEnum(), Common::Common::CommonValue2);
}

QTEST_MAIN(CrossModuleSimulationTest)

#include "tst_cross_module_simulation.moc"
