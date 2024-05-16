// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "backendstestbase.h"
#include "echo.h"
#include "echozoned.h"

#include <QIfServiceManager>
#include <QIfConfiguration>

static QString frontLeftZone = QStringLiteral("FrontLeft");

using namespace Qt::StringLiterals;

BackendsTestBase::BackendsTestBase()
    : QObject()
#if QT_CONFIG(process)
    , m_serverProcess(new QProcess(this))
#endif
    , m_localServer(new QLocalServer(this))
    , m_localSocket(nullptr)
{
#if QT_CONFIG(process)
    m_serverProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_serverProcess, &QProcess::readyRead, this, [this]() {
        while (m_serverProcess->canReadLine()) {
            QByteArray line = m_serverProcess->readLine().chopped(1);
            qInfo().noquote() << "server output:" << QString::fromLocal8Bit(line);
        }
    });
#endif

    QString simulationFile = QFINDTESTDATA("simulation.qml");
    QString simulationDataFile = QFINDTESTDATA("minimal_simulation_data.json");

    //qputenv("QT_LOGGING_RULES", "qt.if.*.debug=true");
    qputenv("QT_FORCE_STDERR_LOGGING", "1");
    qputenv("QTIF_SIMULATION_OVERRIDE", "org.example.echomodule=" + simulationFile.toLocal8Bit());
    qputenv("QTIF_SIMULATION_DATA_OVERRIDE", "org.example.echomodule=" + simulationDataFile.toLocal8Bit());
    Echomodule::registerTypes();
}

void BackendsTestBase::sendCmd(const QByteArray &input)
{
    if (!m_localSocket)
        return;

    m_localSocket->write(input + "\n");
    QVERIFY(m_localSocket->waitForBytesWritten());
    qApp->processEvents();
}

void BackendsTestBase::startServer(QStringList arguments)
{
#if QT_CONFIG(process)
    if (!m_serverExecutable.isEmpty()) {
        qInfo() << "Starting Server Process";
        QVERIFY2(QFile::exists(m_serverExecutable), qPrintable(u"Executable doesn't exist: %1"_s.arg(m_serverExecutable)));
        m_serverProcess->start(m_serverExecutable, arguments);
        QVERIFY2(m_serverProcess->waitForStarted(), qPrintable(u"Process error: %1"_s.arg(m_serverProcess->error())));
    }
#endif
    // Give the simulation some time to (re)connect
    QTest::qWait(100);
    if (!m_localServer->hasPendingConnections())
        QVERIFY(m_localServer->waitForNewConnection(5000));
    while (m_localServer->hasPendingConnections())
        m_localSocket = m_localServer->nextPendingConnection();
    QVERIFY(m_localSocket);

    sendCmd(QTest::currentTestFunction());
}

void BackendsTestBase::ignoreMessage(QtMsgType type, const char *message)
{
    QRegularExpression expression(u".*"_s + message);
    if (m_isSimulationBackend)
        QTest::ignoreMessage(type, expression);
    else
        QTest::ignoreMessage(QtInfoMsg, expression);
}

void BackendsTestBase::cleanupTestData()
{
#if QT_CONFIG(process)
    if (m_serverProcess->state() == QProcess::Running) {
        qInfo() << "Stopping Server Process";

        sendCmd("quit");
        m_serverProcess->waitForFinished(500);

        if (m_serverProcess->state() == QProcess::Running) {
            m_serverProcess->kill();
            QVERIFY(m_serverProcess->waitForFinished());
        }
    }
#endif
    delete m_localSocket;
    m_localSocket = nullptr;

    QIfServiceManager::instance()->unloadAllBackends();
}

void BackendsTestBase::initTestCase_data()
{
    QTest::addColumn<QString>("backend");
    QTest::addColumn<bool>("isSimulation");
    QTest::addColumn<bool>("asyncBackendLoading");
    QTest::addColumn<QString>("serverExecutable");
}

void BackendsTestBase::initTestCase()
{
    if (!m_localServer->listen("qifcmdsocket")) {
        QLocalServer::removeServer("qifcmdsocket");
        QVERIFY2(m_localServer->listen("qifcmdsocket"), qPrintable(m_localServer->errorString()));
    }
}

void BackendsTestBase::init()
{
    QFETCH_GLOBAL(QString, backend);
    QFETCH_GLOBAL(bool, isSimulation);
    QFETCH_GLOBAL(bool, asyncBackendLoading);
    QFETCH_GLOBAL(QString, serverExecutable);

    m_serverExecutable = serverExecutable;
    m_isSimulation = isSimulation;
    m_asyncBackendLoading = asyncBackendLoading;
    m_isSimulationBackend = isSimulation && serverExecutable.isEmpty();

    QVERIFY(QIfConfiguration::setDiscoveryMode(u"org.example.echomodule"_s, m_isSimulationBackend ?
                                                                                QIfAbstractFeature::LoadOnlySimulationBackends :
                                                                                QIfAbstractFeature::LoadOnlyProductionBackends));
    QVERIFY(QIfConfiguration::setPreferredBackends(u"org.example.echomodule"_s, {backend}));
}

void BackendsTestBase::cleanup()
{
    cleanupTestData();
}

void BackendsTestBase::testInit()
{
    QTest::failOnWarning(QRegularExpression(u".*Couldn't retrieve MetaObject for enum parsing:.*"_s));

    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    QCOMPARE(client.lastMessage(), QString());
    QCOMPARE(client.intValue(), 0);
    QCOMPARE(client.floatValue1(), qreal(0.0));
    QCOMPARE(client.floatValue2(), qreal(0.0));
    QCOMPARE(client.stringValue(), QString());
    QCOMPARE(client.comboList(), QVariantList());
    QCOMPARE(client.contact(), Contact());
    QCOMPARE(client.weekDay(), Echomodule::WeekDay());
    QCOMPARE(client.testEnum(), Echomodule::FirstEnumValue);

    QLatin1String lastMessageTestValue("this is the last message");
    int intValueTestValue(789);
    qreal floatValue1TestValue(3.14);
    qreal floatValue2TestValue(2.71);
    QLatin1String stringValueTestValue("test string");
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    Echomodule::WeekDay weekDayTestValue = Echomodule::Wednesday;
    Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;

    if (!m_isSimulationBackend)
        QVERIFY(!client.isInitialized());

    QCOMPARE(client.error(), QIfAbstractFeature::NoError);

    //wait until the client has connected and initial values are set
    QSignalSpy lastMessageChangedSpy(&client, SIGNAL(lastMessageChanged(QString)));
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));

    startServer();

    // A simulation backend is already intialized after it got loaded, there is no IPC and we don't
    // need to wait here until it is initialized after a server start
    if (!m_isSimulationBackend) {
        QVERIFY(initSpy.isValid());
        WAIT_AND_COMPARE(initSpy, 1);
    }
    QVERIFY(client.isInitialized());

    //make sure the change signal is only emitted once with the correct value
    WAIT_AND_COMPARE(lastMessageChangedSpy, 1);
    QCOMPARE(lastMessageChangedSpy.at(0).at(0).toString(), lastMessageTestValue);

    //test that client gets the same values that were set at the server before connection was established
    QCOMPARE(client.lastMessage(), lastMessageTestValue);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QVariantList comboList = client.comboList();
    QCOMPARE(comboList.count(), 2);
    QCOMPARE(comboList[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList[1].value<Combo>(), comboListTestValue[1].value<Combo>());
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(client.weekDay(), weekDayTestValue);
    QCOMPARE(client.testEnum(), testEnumTestValue);


    lastMessageChangedSpy.clear();
    //test that a second instance is also initialized with the correct values
    Echo client2;

    QSignalSpy lastMessageChangedSpy2(&client2, SIGNAL(lastMessageChanged(QString)));
    QSignalSpy initSpy2(&client2, SIGNAL(isInitializedChanged(bool)));

    QVERIFY(client2.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    QVERIFY(initSpy2.isValid());
    WAIT_AND_COMPARE(initSpy2, 1);
    QVERIFY(client2.isInitialized());

    //make sure the change signal is only emitted once with the correct value
    QCOMPARE(lastMessageChangedSpy2.count(), 1);
    QCOMPARE(lastMessageChangedSpy2.at(0).at(0).toString(), lastMessageTestValue);

    //make sure the first instance doesn't emit the change signal again
    QCOMPARE(lastMessageChangedSpy.count(), 0);
}

void BackendsTestBase::testZonedInit()
{
    EchoZoned client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &EchoZoned::serviceObjectChanged);
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    //Just compare a few of them
    QCOMPARE(client.intValue(), 0);
    QCOMPARE(client.varValue(), QVariant());
    QCOMPARE(client.stringValue(), QString());
    QCOMPARE(client.airflowDirection(), Echomodule::AirflowDirections());
    QCOMPARE(client.contact(), Contact());
    QCOMPARE(client.comboList(), QVariantList());
    QCOMPARE(client.UPPERCASEPROPERTY(), qreal(0.0));
    QCOMPARE(client.testEnum(), Echomodule::FirstEnumValue);

    if (!m_isSimulationBackend) {
        QCOMPARE(client.availableZones(), QStringList());
        QVERIFY(!client.isInitialized());
    }
    QCOMPARE(client.error(), QIfAbstractFeature::NoError);

    //wait until the client has connected and initial values are set
    QSignalSpy stringValueChangedSpy(&client, SIGNAL(stringValueChanged(QString)));
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QSignalSpy availableZonesSpy(&client, SIGNAL(availableZonesChanged(QStringList)));

    int intValueTestValue(789);
    QVariant varValueTestValue(789);
    qreal floatValueTestValue(3.14);
    QLatin1String stringValueTestValue("test string");
    QVariantList comboListTestValue(
        { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
         QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    Echomodule::AirflowDirections airflowTestValue = Echomodule::Windshield;
    Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;

    startServer();

    // A simulation backend is already intialized after it got loaded, there is no IPC and we don't
    // need to wait here until it is initialized after a server start
    if (!m_isSimulationBackend) {
        QVERIFY(initSpy.isValid());
        WAIT_AND_COMPARE(initSpy, 1);
        QVERIFY(availableZonesSpy.count());
    }
    QVERIFY(client.isInitialized());
    QCOMPARE(client.availableZones(), QStringList({"FrontLeft"}));

    //make sure the change signal is only emitted once with the correct value
    WAIT_AND_COMPARE(stringValueChangedSpy, 1);
    QCOMPARE(stringValueChangedSpy.at(0).at(0).toString(), stringValueTestValue);

    //test that client gets the same values that were set at the server before connection was established
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(client.varValue(), varValueTestValue);
    QCOMPARE(client.stringValue(), stringValueTestValue);

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);
    QCOMPARE(zone->airflowDirection(), airflowTestValue);
    QCOMPARE(zone->contact(), contactTestValue);
    QVariantList comboList = zone->comboList();
    QCOMPARE(comboList.count(), 2);
    QCOMPARE(comboList[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList[1].value<Combo>(), comboListTestValue[1].value<Combo>());
    QCOMPARE(zone->UPPERCASEPROPERTY(), floatValueTestValue);
    QCOMPARE(zone->testEnum(), testEnumTestValue);

    stringValueChangedSpy.clear();
    //test that a second instance is also initialized with the correct values
    EchoZoned client2;

    QSignalSpy stringValueChangedSpy2(&client2, SIGNAL(stringValueChanged(QString)));
    QSignalSpy initSpy2(&client2, SIGNAL(isInitializedChanged(bool)));

    QVERIFY(client2.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    QVERIFY(initSpy2.isValid());
    WAIT_AND_COMPARE(initSpy2, 1);
    QVERIFY(client2.isInitialized());

    //make sure the change signal is only emitted once with the correct value
    QCOMPARE(stringValueChangedSpy2.count(), 1);
    QCOMPARE(stringValueChangedSpy2.at(0).at(0).toString(), stringValueTestValue);

    //make sure the first instance doesn't emit the change signal again
    QCOMPARE(stringValueChangedSpy.count(), 0);
}

void BackendsTestBase::testReconnect()
{
    if (m_serverExecutable.isEmpty())
        QSKIP("The reconnection test only makes sense with a server process");


    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(zonedInitSpy, 1);
    QVERIFY(zonedClient.isInitialized());

    //test disconnection
    QCOMPARE(client.error(), QIfAbstractFeature::NoError);
    QCOMPARE(zonedClient.error(), QIfAbstractFeature::NoError);
    QSignalSpy disconnectSpy(&client, SIGNAL(errorChanged(QIfAbstractFeature::Error, QString)));
    QSignalSpy zonedDisconnectSpy(&zonedClient, SIGNAL(errorChanged(QIfAbstractFeature::Error, QString)));
    QVERIFY(disconnectSpy.isValid());
    QVERIFY(zonedDisconnectSpy.isValid());

#if QT_CONFIG(process)
    m_serverProcess->kill();
    m_serverProcess->waitForFinished();
#endif

    WAIT_AND_COMPARE(disconnectSpy, 1);
    QCOMPARE(client.error(), QIfAbstractFeature::Unknown);
    WAIT_AND_COMPARE(zonedDisconnectSpy, 1);
    QCOMPARE(zonedClient.error(), QIfAbstractFeature::Unknown);

    //test that a remote call fails on a disconnected replica
    QIfPendingReply<QString> idReply = client.id();
    QVERIFY(idReply.isResultAvailable() && !idReply.watcher()->isSuccessful());

    QIfPendingReply<QString> zonedIdReply = zonedClient.id();
    QVERIFY(zonedIdReply.isResultAvailable() && !zonedIdReply.watcher()->isSuccessful());

    //test reconnection
    QSignalSpy reconnectSpy(&client, SIGNAL(errorChanged(QIfAbstractFeature::Error, QString)));
    QVERIFY(reconnectSpy.isValid());
    QSignalSpy zonedReconnectSpy(&zonedClient, SIGNAL(errorChanged(QIfAbstractFeature::Error, QString)));
    QVERIFY(zonedReconnectSpy.isValid());

    startServer();

    WAIT_AND_COMPARE(reconnectSpy, 1);
    QCOMPARE(client.error(), QIfAbstractFeature::NoError);
    WAIT_AND_COMPARE(zonedReconnectSpy, 1);
    QCOMPARE(zonedClient.error(), QIfAbstractFeature::NoError);
}

void BackendsTestBase::testClient2Server()
{
    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    client.setIntValue(intValueTestValue);
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy floatValue1Spy(&client, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    client.setFloatValue1(floatValue1TestValue);
    WAIT_AND_COMPARE(floatValue1Spy, 1);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QSignalSpy floatValue2Spy(&client, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 3.1415;
    client.setFloatValue2(floatValue2TestValue);
    WAIT_AND_COMPARE(floatValue2Spy, 1);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QSignalSpy stringValueSpy(&client, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QStringLiteral("hello test");
    client.setStringValue(stringValueTestValue);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy comboListSpy(&client, SIGNAL(comboListChanged(QVariantList)));
    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
    client.setComboList(comboListTestValue);
    WAIT_AND_COMPARE(comboListSpy, 1);
    QCOMPARE(client.comboList().count(),comboListTestValue.count());
    QCOMPARE(client.comboList().at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(client.comboList().at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());

    QSignalSpy contactSpy(&client, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    client.setContact(contactTestValue);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy weekDaySpy(&client, SIGNAL(weekDayChanged(Echomodule::DaysOfTheWeek)));
    QVERIFY(weekDaySpy.isValid());
    Echomodule::DaysOfTheWeek weekDayTestValue = Echomodule::Thursday;
    client.setWeekDay(weekDayTestValue);
    WAIT_AND_COMPARE(weekDaySpy, 1);
    QCOMPARE(client.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<Echomodule::DaysOfTheWeek>(), weekDayTestValue);

    QSignalSpy testEnumSpy(&client, SIGNAL(testEnumChanged(Echomodule::TestEnum)));
    QVERIFY(testEnumSpy.isValid());
    Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;
    client.setTestEnum(testEnumTestValue);
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(client.testEnum(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<Echomodule::TestEnum>(), testEnumTestValue);
}

void BackendsTestBase::testZonedClient2Server()
{
    EchoZoned client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &EchoZoned::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test properties
    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    client.setIntValue(intValueTestValue);
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QSignalSpy UPPERCASEPROPERTYSpy(&client, SIGNAL(UPPERCASEPROPERTYChanged(qreal)));
    QVERIFY(UPPERCASEPROPERTYSpy.isValid());
    qreal floatValueTestValue = 1234.5678;
    client.setUPPERCASEPROPERTY(floatValueTestValue);
    WAIT_AND_COMPARE(UPPERCASEPROPERTYSpy, 1);
    QCOMPARE(client.UPPERCASEPROPERTY(), floatValueTestValue);
    QCOMPARE(UPPERCASEPROPERTYSpy[0][0].toReal(), floatValueTestValue);

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QSignalSpy stringValueSpy(zone, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QStringLiteral("hello test");
    zone->setStringValue(stringValueTestValue);
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(zone->stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QSignalSpy comboListSpy(zone, SIGNAL(comboListChanged(QVariantList)));
    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
    zone->setComboList(comboListTestValue);
    WAIT_AND_COMPARE(comboListSpy, 1);
    QVariantList comboList = zone->comboList();
    QCOMPARE(comboList.count(),comboListTestValue.count());
    QCOMPARE(comboList.count(), 2);
    QCOMPARE(comboList.at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList.at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs.count(), 2);
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());

    QSignalSpy contactSpy(zone, SIGNAL(contactChanged(Contact)));
    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    zone->setContact(contactTestValue);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(zone->contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QSignalSpy airflowSpy(zone, SIGNAL(airflowDirectionChanged(Echomodule::AirflowDirections)));
    QVERIFY(airflowSpy.isValid());
    Echomodule::AirflowDirections airflowTestValue = Echomodule::Windshield;
    zone->setAirflowDirection(airflowTestValue);
    WAIT_AND_COMPARE(airflowSpy, 1);
    QCOMPARE(zone->airflowDirection(), airflowTestValue);
    QCOMPARE(airflowSpy[0][0].value<Echomodule::AirflowDirections>(), airflowTestValue);

    QSignalSpy testEnumSpy(zone, SIGNAL(testEnumChanged(Echomodule::TestEnum)));
    QVERIFY(testEnumSpy.isValid());
    Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;
    zone->setTestEnum(testEnumTestValue);
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(zone->testEnum(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<Echomodule::TestEnum>(), testEnumTestValue);
}

void BackendsTestBase::testServer2Client()
{
    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QSignalSpy floatValue2Spy(&client, SIGNAL(floatValue2Changed(qreal)));
    QSignalSpy floatValue1Spy(&client, SIGNAL(floatValue1Changed(qreal)));
    QSignalSpy stringValueSpy(&client, SIGNAL(stringValueChanged(QString)));
    QSignalSpy comboListSpy(&client, SIGNAL(comboListChanged(QVariantList)));
    QSignalSpy contactSpy(&client, SIGNAL(contactChanged(Contact)));
    QSignalSpy weekDaySpy(&client, SIGNAL(weekDayChanged(Echomodule::DaysOfTheWeek)));
    QSignalSpy testEnumSpy(&client, SIGNAL(testEnumChanged(Echomodule::TestEnum)));

    // Send a signal to the server that new values should be published
    sendCmd("changeProperties");

    // Check all retrieved values
    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QVERIFY(floatValue1Spy.isValid());
    qreal floatValue1TestValue = 1234.5678;
    WAIT_AND_COMPARE(floatValue1Spy, 1);
    QCOMPARE(client.floatValue1(), floatValue1TestValue);
    QCOMPARE(floatValue1Spy[0][0].toReal(), floatValue1TestValue);

    QVERIFY(floatValue2Spy.isValid());
    qreal floatValue2TestValue = 234.678;
    WAIT_AND_COMPARE(floatValue2Spy, 1);
    QCOMPARE(client.floatValue2(), floatValue2TestValue);
    QCOMPARE(floatValue2Spy[0][0].toReal(), floatValue2TestValue);

    QVERIFY(stringValueSpy.isValid());
    QLatin1String stringValueTestValue("hello test");
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(client.stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
    WAIT_AND_COMPARE(comboListSpy, 1);
    QCOMPARE(client.comboList().count(),comboListTestValue.count());
    QCOMPARE(client.comboList().at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(client.comboList().at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());

    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(client.contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QVERIFY(weekDaySpy.isValid());
    Echomodule::WeekDay weekDayTestValue = Echomodule::Friday;
    WAIT_AND_COMPARE(weekDaySpy, 1);

    QCOMPARE(client.weekDay(), weekDayTestValue);
    QCOMPARE(weekDaySpy[0][0].value<Echomodule::DaysOfTheWeek>(), weekDayTestValue);

    QVERIFY(testEnumSpy.isValid());
    Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(client.testEnum(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<Echomodule::TestEnum>(), testEnumTestValue);
}

void BackendsTestBase::testZonedServer2Client()
{
    EchoZoned client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &EchoZoned::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QSignalSpy intValueSpy(&client, SIGNAL(intValueChanged(int)));
    QSignalSpy UPPERCASEPROPERTYSpy(&client, SIGNAL(UPPERCASEPROPERTYChanged(qreal)));
    QSignalSpy stringValueSpy(zone, SIGNAL(stringValueChanged(QString)));
    QSignalSpy comboListSpy(zone, SIGNAL(comboListChanged(QVariantList)));
    QSignalSpy contactSpy(zone, SIGNAL(contactChanged(Contact)));
    QSignalSpy airflowSpy(zone, SIGNAL(airflowDirectionChanged(Echomodule::AirflowDirections)));
    QSignalSpy testEnumSpy(zone, SIGNAL(testEnumChanged(Echomodule::TestEnum)));

    QTest::qSleep(200);
    // Send a signal to the server that new values should be published
    sendCmd("changeZoneProperties");

    QVERIFY(intValueSpy.isValid());
    int intValueTestValue = 12345;
    WAIT_AND_COMPARE(intValueSpy, 1);
    QCOMPARE(client.intValue(), intValueTestValue);
    QCOMPARE(intValueSpy[0][0].toInt(), intValueTestValue);

    QVERIFY(UPPERCASEPROPERTYSpy.isValid());
    qreal floatValueTestValue = 1234.5678;
    WAIT_AND_COMPARE(UPPERCASEPROPERTYSpy, 1);
    QCOMPARE(client.UPPERCASEPROPERTY(), floatValueTestValue);
    QCOMPARE(UPPERCASEPROPERTYSpy[0][0].toReal(), floatValueTestValue);


    QVERIFY(stringValueSpy.isValid());
    QString stringValueTestValue = QStringLiteral("hello test");
    WAIT_AND_COMPARE(stringValueSpy, 1);
    QCOMPARE(zone->stringValue(), stringValueTestValue);
    QCOMPARE(stringValueSpy[0][0].toString(), stringValueTestValue);

    QVERIFY(comboListSpy.isValid());
    QVariantList comboListTestValue(
                    { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Thursday)),
                      QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Tuesday)) });
    WAIT_AND_COMPARE(comboListSpy, 1);
    QVariantList comboList = zone->comboList();
    QCOMPARE(comboList.count(), comboListTestValue.count());
    QCOMPARE(comboList.at(0).value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(comboList.at(1).value<Combo>(), comboListTestValue[1].value<Combo>());
    QVariantList signalArgs = comboListSpy[0][0].toList();
    QCOMPARE(signalArgs[0].value<Combo>(), comboListTestValue[0].value<Combo>());
    QCOMPARE(signalArgs[1].value<Combo>(), comboListTestValue[1].value<Combo>());

    QVERIFY(contactSpy.isValid());
    Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
    WAIT_AND_COMPARE(contactSpy, 1);
    QCOMPARE(zone->contact(), contactTestValue);
    QCOMPARE(contactSpy[0][0].value<Contact>(), contactTestValue);

    QVERIFY(airflowSpy.isValid());
    Echomodule::AirflowDirections airflowTestValue = Echomodule::Windshield;
    WAIT_AND_COMPARE(airflowSpy, 1);
    QCOMPARE(zone->airflowDirection(), airflowTestValue);
    QCOMPARE(airflowSpy[0][0].value<Echomodule::AirflowDirections>(), airflowTestValue);

    QVERIFY(testEnumSpy.isValid());
    Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;
    WAIT_AND_COMPARE(testEnumSpy, 1);
    QCOMPARE(zone->testEnum(), testEnumTestValue);
    QCOMPARE(testEnumSpy[0][0].value<Echomodule::TestEnum>(), testEnumTestValue);
}

void BackendsTestBase::testSlots()
{
    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();


    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test slots by calling them on the client
    QLatin1String echoTestValue("this will be echoed");
    QIfPendingReply<QString> echoReply = client.echo(echoTestValue);
    if (!echoReply.isResultAvailable()) {
        QSignalSpy echoReplySpy(echoReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(echoReplySpy, 1);
    }
    QCOMPARE(echoReply.reply(), echoTestValue);

    QIfPendingReply<QString> idReply = client.id();
    if (!idReply.isResultAvailable()) {
        QSignalSpy idReplySpy(idReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(idReplySpy, 1);
    }
    QCOMPARE(idReply.reply(), QStringLiteral("id123"));

    Combo expectedCombo(Contact(QStringLiteral("Antti"), 34, true, QVariant()), Echomodule::Friday);
    QIfPendingReply<Combo> comboReply = client.getCombo();
    if (!comboReply.isResultAvailable()) {
        QSignalSpy comboReplySpy(comboReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(comboReplySpy, 1);
    }
    QCOMPARE(comboReply.reply(), expectedCombo);

    QIfPendingReply<void> voidReply = client.voidSlot();
    if (!voidReply.isResultAvailable()) {
        QSignalSpy voidReplySpy(voidReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(voidReplySpy, 1);
    }

    int voidSlot2TestValue = 776;
    QIfPendingReply<void> void2Reply = client.voidSlot2(voidSlot2TestValue);
    if (!void2Reply.isResultAvailable()) {
        QSignalSpy void2ReplySpy(void2Reply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(void2ReplySpy, 1);
    }

    Echomodule::AirflowDirections flagTestValue = Echomodule::Dashboard;
    QIfPendingReply<Echomodule::AirflowDirections> flagMethodReply = client.flagMethod(flagTestValue);
    if (!flagMethodReply.isResultAvailable()) {
        QSignalSpy flagMethodReplySpy(flagMethodReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(flagMethodReplySpy, 1);
    }
    QCOMPARE(flagMethodReply.reply(), flagTestValue);

    Echomodule::TestEnum enumTestValue = Echomodule::SecondEnumValue;
    QIfPendingReply<Echomodule::TestEnum> enumMethodReply = client.enumMethod(enumTestValue);
    if (!enumMethodReply.isResultAvailable()) {
        QSignalSpy enumMethodReplySpy(enumMethodReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(enumMethodReplySpy, 1);
    }
    QCOMPARE(enumMethodReply.reply(), enumTestValue);
}

void BackendsTestBase::testZonedSlots()
{
    EchoZoned client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &EchoZoned::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();


    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test slots by calling them on the client
    QLatin1String echoTestValue("this will be echoed");
    QIfPendingReply<QString> echoReply = client.echo(echoTestValue);
    if (!echoReply.isResultAvailable()) {
        QSignalSpy echoReplySpy(echoReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(echoReplySpy, 1);
    }
    QCOMPARE(echoReply.reply(), echoTestValue);

    QIfPendingReply<QString> idReply = client.id();
    if (!idReply.isResultAvailable()) {
        QSignalSpy idReplySpy(idReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(idReplySpy, 1);
    }
    QCOMPARE(idReply.reply(), QStringLiteral("id123"));

    EchoZoned *zone = qobject_cast<EchoZoned*>(client.zoneAt(frontLeftZone));
    QVERIFY(zone);

    Combo expectedCompo(Contact(QStringLiteral("Antti"), 34, true, QVariant()), Echomodule::Friday);
    QIfPendingReply<Combo> comboReply = zone->getCombo();
    if (!comboReply.isResultAvailable()) {
        QSignalSpy comboReplySpy(comboReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(comboReplySpy, 1);
    }
    QCOMPARE(comboReply.reply(), expectedCompo);

    Echomodule::AirflowDirections flagTestValue = Echomodule::Dashboard;
    QIfPendingReply<Echomodule::AirflowDirections> flagMethodReply = zone->flagMethod(flagTestValue);
    if (!flagMethodReply.isResultAvailable()) {
        QSignalSpy flagMethodReplySpy(flagMethodReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(flagMethodReplySpy, 1);
    }
    QCOMPARE(flagMethodReply.reply(), flagTestValue);

    Echomodule::TestEnum enumTestValue = Echomodule::SecondEnumValue;
    QIfPendingReply<Echomodule::TestEnum> enumMethodReply = zone->enumMethod(enumTestValue);
    if (!enumMethodReply.isResultAvailable()) {
        QSignalSpy enumMethodReplySpy(enumMethodReply.watcher(), SIGNAL(replySuccess()));
        WAIT_AND_COMPARE(enumMethodReplySpy, 1);
    }
    QCOMPARE(enumMethodReply.reply(), enumTestValue);
}

void BackendsTestBase::testMultipleSlotCalls()
{
    Echo client;
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    QVERIFY(client.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    startServer();


    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //test the pending replies by calling the same slot with 3 different values
    QLatin1String echoTestValue("first");
    QLatin1String echoTestValue2("second");
    QLatin1String echoTestValue3("third");
    QIfPendingReply<QString> echoReply = client.echo(echoTestValue);
    QIfPendingReply<QString> echoReply2 = client.echo(echoTestValue2);
    QIfPendingReply<QString> echoReply3 = client.echo(echoTestValue3);
    if (!echoReply.isResultAvailable() && !echoReply2.isResultAvailable() && !echoReply3.isResultAvailable()) {
        QSignalSpy echoReplySpy(echoReply.watcher(), SIGNAL(replySuccess()));
        QSignalSpy echoReplySpy2(echoReply2.watcher(), SIGNAL(replySuccess()));
        QSignalSpy echoReplySpy3(echoReply3.watcher(), SIGNAL(replySuccess()));
        echoReplySpy3.wait();
        QCOMPARE(echoReplySpy.count(), 1);
        QCOMPARE(echoReplySpy2.count(), 1);
        QCOMPARE(echoReplySpy3.count(), 1);
    }
    QCOMPARE(echoReply.reply(), echoTestValue);
    QCOMPARE(echoReply2.reply(), echoTestValue2);
    QCOMPARE(echoReply3.reply(), echoTestValue3);

    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(zonedInitSpy, 1);
    QVERIFY(zonedClient.isInitialized());

    EchoZoned *zone = qobject_cast<EchoZoned*>(zonedClient.zoneAt(frontLeftZone));
    QVERIFY(zone);

    QIfPendingReply<QString> echoZonedReply = zone->echo(echoTestValue);
    QIfPendingReply<QString> echoZonedReply2 = zone->echo(echoTestValue2);
    QIfPendingReply<QString> echoZonedReply3 = zone->echo(echoTestValue3);
    if (!echoZonedReply.isResultAvailable() && !echoZonedReply2.isResultAvailable() && !echoZonedReply3.isResultAvailable()) {
        QSignalSpy echoZonedReplySpy(echoZonedReply.watcher(), SIGNAL(replySuccess()));
        QSignalSpy echoZonedReplySpy2(echoZonedReply2.watcher(), SIGNAL(replySuccess()));
        QSignalSpy echoZonedReplySpy3(echoZonedReply3.watcher(), SIGNAL(replySuccess()));
        echoZonedReplySpy3.wait();
        QCOMPARE(echoZonedReplySpy.count(), 1);
        QCOMPARE(echoZonedReplySpy2.count(), 1);
        QCOMPARE(echoZonedReplySpy3.count(), 1);
    }
    QCOMPARE(echoZonedReply.reply(), echoTestValue);
    QCOMPARE(echoZonedReply2.reply(), echoTestValue2);
    QCOMPARE(echoZonedReply3.reply(), echoTestValue3);
}

void BackendsTestBase::testAsyncSlotResults()
{
    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    // test the timer() function which uses a pendingReply on the server side to return the
    // function when the timer is finished.
    QIfPendingReply<void> reply = client.timer(1000);
    QIfPendingReply<void> reply2 = client.timer(500);
    QSignalSpy echoReplySpy(reply.watcher(), SIGNAL(replySuccess()));
    QSignalSpy echoReplySpy2(reply2.watcher(), SIGNAL(replySuccess()));

    //Wait for the second reply to return first. Verify the other reply is not yet ready.
    echoReplySpy2.wait();
    QCOMPARE(echoReplySpy2.count(), 1);
    QCOMPARE(echoReplySpy.count(), 0);

    //Wait for the first reply and verify both replies were sent.
    echoReplySpy.wait();
    QCOMPARE(echoReplySpy2.count(), 1);
    QCOMPARE(echoReplySpy.count(), 1);

    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(zonedInitSpy, 1);
    QVERIFY(zonedClient.isInitialized());

    EchoZoned *zone = qobject_cast<EchoZoned*>(zonedClient.zoneAt(frontLeftZone));
    QVERIFY(zone);

    // test the timer() function which uses a pendingReply on the server side to return the
    // function when the timer is finished.
    QIfPendingReply<QString> zonedReply = zonedClient.timer(1000);
    QIfPendingReply<QString> zonedReply2 = zone->timer(500);
    QSignalSpy zonedEchoReplySpy(zonedReply.watcher(), SIGNAL(replySuccess()));
    QSignalSpy zonedEchoReplySpy2(zonedReply2.watcher(), SIGNAL(replySuccess()));

    //Wait for the second reply to return first. Verify the other reply is not yet ready.
    zonedEchoReplySpy2.wait();
    QCOMPARE(zonedEchoReplySpy2.count(), 1);
    QCOMPARE(zonedEchoReplySpy.count(), 0);
    QCOMPARE(zonedReply2.value(), frontLeftZone);

    //Wait for the first reply and verify both replies were sent.
    zonedEchoReplySpy.wait();
    QCOMPARE(zonedEchoReplySpy2.count(), 1);
    QCOMPARE(zonedEchoReplySpy.count(), 1);
    QCOMPARE(zonedReply.value(), QString());
}

void BackendsTestBase::testSignals()
{
    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    EchoZoned zonedClient;

    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(zonedInitSpy, 1);
    QVERIFY(zonedClient.isInitialized());

    EchoZoned *zone = qobject_cast<EchoZoned*>(zonedClient.zoneAt(frontLeftZone));
    QVERIFY(zone);

    //test custom signals (other than property notifiers) from server to client
    QSignalSpy anotherChangedSpy(&client, SIGNAL(anotherChanged(AnotherStruct)));
    QSignalSpy foobarSpy(&client, SIGNAL(foobar(QString)));
    QSignalSpy somethingSpy(&client, SIGNAL(somethingHappened()));
    QSignalSpy zonedAnotherChangedSpy(&zonedClient, SIGNAL(anotherChanged(AnotherStruct)));
    QSignalSpy zonedFoobarSpy(zone, SIGNAL(foobar(QString)));
    QSignalSpy zonedSomethingSpy(zone, SIGNAL(somethingHappened()));

    zonedClient.setIntValue(1234);

    // Send a signal to the server that new values should be published
    sendCmd("emitSignals");

    QVERIFY(anotherChangedSpy.isValid());
    AnotherStruct anotherTestValue(7);
    WAIT_AND_COMPARE(anotherChangedSpy, 1);
    QCOMPARE(anotherChangedSpy[0][0].value<AnotherStruct>(), anotherTestValue);

    QVERIFY(foobarSpy.isValid());
    QLatin1String foobarTestValue("foo and bar");
    WAIT_AND_COMPARE(foobarSpy, 1);
    QCOMPARE(foobarSpy[0][0].toString(), foobarTestValue);

    QVERIFY(somethingSpy.isValid());;
    WAIT_AND_COMPARE(somethingSpy, 1);

    QVERIFY(zonedAnotherChangedSpy.isValid());
    WAIT_AND_COMPARE(zonedAnotherChangedSpy, 1);
    QCOMPARE(zonedAnotherChangedSpy[0][0].value<AnotherStruct>(), anotherTestValue);


    QVERIFY(zonedFoobarSpy.isValid());
    WAIT_AND_COMPARE(zonedFoobarSpy, 1);
    QCOMPARE(zonedFoobarSpy[0][0].toString(), foobarTestValue);

    QVERIFY(zonedSomethingSpy.isValid());;
    WAIT_AND_COMPARE(zonedSomethingSpy, 1);
}

void BackendsTestBase::testModel()
{
    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    //Give QtRO time to actually call our server side
    QTest::qWait(200);

    QIfPagingModel* model = client.contactList();
    QVERIFY(model->isInitialized());
    QCOMPARE(model->rowCount(), 0);

    //Test inserting a row
    Contact testContact(QStringLiteral("Mr A."), 20, false, "foo");
    QSignalSpy countSpy(model, SIGNAL(countChanged()));

    // Send a signal to the server that new values should be published
    sendCmd("insert");

    WAIT_AND_COMPARE(countSpy, 1);
    QCOMPARE(model->rowCount(), 1);
    QCOMPARE(model->at<Contact>(0), testContact);
    countSpy.clear();

    //test updating a row
    QSignalSpy changedSpy(model, SIGNAL(dataChanged( QModelIndex, QModelIndex, QVector<int>)));
    Contact updatedContact(QStringLiteral("Mr B."), 30, true, QVariant());
    sendCmd("update");

    WAIT_AND_COMPARE(changedSpy, 1);
    QCOMPARE(model->rowCount(), 1);
    QCOMPARE(model->at<Contact>(0), updatedContact);

    //Test removing a row
    sendCmd("remove");
    WAIT_AND_COMPARE(countSpy, 1);
    QCOMPARE(model->rowCount(), 0);
}

void BackendsTestBase::testSimulationData()
{
    if (!m_isSimulation)
        QSKIP("This test is only for simulation backend and simulation servers");

    Echo client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy serviceObjectChangedSpy(&client, &Echo::serviceObjectChanged);
    QSignalSpy initSpy(&client, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();
    WAIT_AND_COMPARE(serviceObjectChangedSpy, 1);

    startServer();

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    QSignalSpy floatValue1Spy(&client, SIGNAL(floatValue1Changed(qreal)));
    QVERIFY(floatValue1Spy.isValid());
    ignoreMessage(QtCriticalMsg, "SIMULATION changing floatValue1 is not possible: provided: 1 constraint: >= 2");

    client.setFloatValue1(1);
    floatValue1Spy.wait(500);
    QCOMPARE(floatValue1Spy.count(), 0);
    QCOMPARE(client.floatValue1(), 0);

    QSignalSpy floatValue2Spy(&client, SIGNAL(floatValue2Changed(qreal)));
    QVERIFY(floatValue2Spy.isValid());
    ignoreMessage(QtCriticalMsg, "SIMULATION changing floatValue2 is not possible: provided: 400 constraint: <= 300");

    client.setFloatValue2(400);
    floatValue2Spy.wait(500);
    QCOMPARE(floatValue2Spy.count(), 0);
    QCOMPARE(client.floatValue2(), 0);


    EchoZoned zonedClient;
    QSignalSpy zonedInitSpy(&zonedClient, SIGNAL(isInitializedChanged(bool)));
    QVERIFY(zonedInitSpy.isValid());
    QVERIFY(zonedClient.startAutoDiscovery() > QIfAbstractFeature::ErrorWhileLoading);

    //wait until the client has connected and initial values are set
    WAIT_AND_COMPARE(zonedInitSpy, 1);
    QVERIFY(zonedClient.isInitialized());

    EchoZoned *zone =  qobject_cast<EchoZoned*>(zonedClient.zoneAt(frontLeftZone));

    QSignalSpy intValueSpy(zone, SIGNAL(intValueChanged(int)));
    QVERIFY(intValueSpy.isValid());
    ignoreMessage(QtCriticalMsg, "SIMULATION changing intValue is not possible: provided: 1 constraint: \\[10-33\\]");

    zone->setIntValue(1);
    intValueSpy.wait(500);
    QCOMPARE(intValueSpy.count(), 0);
    QCOMPARE(zone->intValue(), 0);

    ignoreMessage(QtCriticalMsg, "SIMULATION changing intValue is not possible: provided: 40 constraint: \\[10-33\\]");

    zone->setIntValue(40);
    intValueSpy.wait(500);
    QCOMPARE(intValueSpy.count(), 0);
    QCOMPARE(zone->intValue(), 0);

    QSignalSpy rangedValueSpy(zone, SIGNAL(rangedValueChanged(int)));
    QVERIFY(rangedValueSpy.isValid());
    ignoreMessage(QtCriticalMsg, "SIMULATION changing rangedValue is not possible: provided: 1 constraint: \\[10-15\\]");

    zone->setRangedValue(1);
    rangedValueSpy.wait(500);
    QCOMPARE(rangedValueSpy.count(), 0);
    QCOMPARE(zone->rangedValue(), 0);

    ignoreMessage(QtCriticalMsg, "SIMULATION changing rangedValue is not possible: provided: 40 constraint: \\[10-15\\]");

    zone->setRangedValue(40);
    rangedValueSpy.wait(500);
    QCOMPARE(rangedValueSpy.count(), 0);
    QCOMPARE(zone->rangedValue(), 0);

    QSignalSpy stringValueSpy(zone, SIGNAL(stringValueChanged(QString)));
    QVERIFY(stringValueSpy.isValid());
    ignoreMessage(QtCriticalMsg, "SIMULATION changing stringValue is not possible: provided: 12345 constraint: \\[\"hello test\",\"test string\",\"three\"\\]");

    zone->setStringValue(u"12345"_s);
    stringValueSpy.wait(500);
    QCOMPARE(stringValueSpy.count(), 0);
    QCOMPARE(zone->stringValue(), QString());

    QSignalSpy unsupportedValueSpy(zone, SIGNAL(unsupportedValueChanged(QString)));
    QVERIFY(unsupportedValueSpy.isValid());
    ignoreMessage(QtCriticalMsg, "SIMULATION changing unsupportedValue is not possible: provided: 12345 constraint: unsupported");

    zone->setUnsupportedValue(u"12345"_s);
    unsupportedValueSpy.wait(500);
    QCOMPARE(unsupportedValueSpy.count(), 0);
    QCOMPARE(zone->stringValue(), QString());
}
