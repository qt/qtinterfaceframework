// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "backendstestbase.h"
#include "echo.h"
#include "echozoned.h"

using namespace Qt::StringLiterals;

class QtRoBackendTest : public BackendsTestBase
{
    Q_OBJECT

private slots:
    void initTestCase_data() override
    {
        BackendsTestBase::initTestCase_data();
        QDir currentDir = QDir::current();

        QTest::newRow("qtro-static-backend") << "org.example.echomodule_qtro_static" << false << currentDir.absoluteFilePath(u"org-example-echo-qtro-server"_s + exeSuffix);
        QTest::newRow("qtro-server") << "echo_backend_qtro" << false << currentDir.absoluteFilePath(u"org-example-echo-qtro-server"_s + exeSuffix);
        QTest::newRow("qtro-simulation-server") << "echo_backend_qtro" << true << currentDir.absoluteFilePath(u"org-example-echo-qtro-simulation-server"_s + exeSuffix);
    }

    void testRemoteObjectsConfig()
    {
        if (m_isSimulationBackend)
            QSKIP("This test is only for remoteobject");

        Echo client;
        QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
        QVERIFY(initSpy.isValid());
        QVERIFY(client.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

        EchoZoned zonedClient;
        QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
        QVERIFY(zonedInitSpy.isValid());
        zonedClient.setServiceObject(client.serviceObject());

        // Test with same URL for all services
        startServer({"--serverUrl", "local:myTestUrl"});
        client.serviceObject()->updateServiceSettings(QVariantMap({{QString("connectionUrl"), QVariant("local:myTestUrl")}}));

        //wait until the client has connected and initial values are set
        WAIT_AND_COMPARE(initSpy, 1);
        QVERIFY(client.isInitialized());

        WAIT_AND_COMPARE(zonedInitSpy, 1);
        QVERIFY(zonedClient.isInitialized());

        cleanupTestData();
        QVERIFY(!client.isInitialized());
        QVERIFY(!zonedClient.isInitialized());
        initSpy.clear();
        zonedInitSpy.clear();
        QVERIFY(client.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);
        zonedClient.setServiceObject(client.serviceObject());

        // Test with conf file, which uses different URLs for different services
        startServer({"--serverConf", QFINDTESTDATA("server.conf")});
        client.serviceObject()->updateServiceSettings(QVariantMap({
            {QString("org.example.echomodule.EchoZoned"), QVariantMap({{QString("connectionUrl"), QVariant("local:echozoned")}})},
            {QString("org.example.echomodule"), QVariantMap({{QString("connectionUrl"), QVariant("local:echomoduleconf")}})}
        }));


        //wait until the client has connected and initial values are set
        WAIT_AND_COMPARE(initSpy, 1);
        QVERIFY(client.isInitialized());

        WAIT_AND_COMPARE(zonedInitSpy, 1);
        QVERIFY(zonedClient.isInitialized());
    }
};

QTEST_MAIN(QtRoBackendTest)

#include "tst_qtro_backend.moc"
