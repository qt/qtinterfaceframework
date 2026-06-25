// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "mqtttestbase.h"
#include "echozonedstatic.h"

#include <QProcess>
#include <QTcpSocket>

using namespace Qt::StringLiterals;

MqttTestBase::MqttTestBase()
    : m_brokerProcess(new QProcess(this))
{
    if (!checkForRunningBroker()) {
        qInfo() << "No Broker running. Trying to start a test broker.";
        const QString brokerLocation = QFile::decodeName(qgetenv("MQTT_TEST_BROKER_LOCATION"));
        if (brokerLocation.isEmpty()) {
            setSkippedTests({
                    {"initTestCase", "No test broker provided. Skipping test"}
            });
            return;
        }

        // Start the paho test broker
        QString python = QFile::decodeName(qgetenv("PYTHON3_PATH"));
        if (!python.isEmpty()) {
#ifdef Q_OS_WIN
            python += QLatin1String("/python.exe");
#else
            python += QLatin1String("/python3");
#endif
            if (!QFileInfo::exists(python)) {
                qWarning() << "Could not find Python at:" << python << ". Assuming it in PATH.";
                python.clear();
            }
        }

        if (python.isEmpty()) {
#ifdef Q_OS_WIN
            python = QLatin1String("python.exe");
#else
            python = QLatin1String("python3");
#endif
        }

        QStringList arguments = {brokerLocation};

        qDebug() << "Launching broker:" << python << arguments;
        m_brokerProcess->start(python, arguments);
        if (!m_brokerProcess->waitForStarted())
            qFatal("Could not start MQTT test broker.");

        if (!checkForRunningBroker())
            qFatal("Could not launch MQTT test broker.");
    }

    setSkippedTests({
                     {"testReconnect", "We can't test this without stopping the broker"},
                     {"testModel", "The mqtt_backend doesn't support models for now!"},
    });
}

MqttTestBase::~MqttTestBase()
{
    m_brokerProcess->kill();
    m_brokerProcess->waitForFinished();
}

bool MqttTestBase::checkForRunningBroker()
{
    qInfo() << "Checking if a local broker is running...";
    const int maxTries = 6;
    // Give the server some time to initialize, not only launch.
    // Cannot use QTRY_*
    for (int tryCounter = 0; tryCounter < maxTries; ++tryCounter) {
        QTcpSocket socket;
        socket.connectToHost(QLatin1String("localhost"), 1883);

        if (socket.waitForConnected(5000))
            return true;
        QTest::qWait(500);
    }

    return false;
}

void MqttTestBase::testStaticZones()
{
    CHECK_SKIP();

    EchoZonedStatic client;
    client.setAsynchronousBackendLoading(m_asyncBackendLoading);
    QSignalSpy initSpy(&client, &QIfAbstractFeature::isInitializedChanged);
    QSignalSpy availableZonesSpy(&client, &EchoZonedStatic::availableZonesChanged);
    QVERIFY(initSpy.isValid());
    client.startAutoDiscovery();

    WAIT_AND_COMPARE(initSpy, 1);
    QVERIFY(client.isInitialized());

    QVERIFY(availableZonesSpy.count() >= 1);
    QCOMPARE(client.availableZones(), QStringList({ u"FrontLeft"_s, u"Rear"_s }));
    QVERIFY(qobject_cast<EchoZonedStatic*>(client.zoneAt(u"FrontLeft"_s)));
    QVERIFY(qobject_cast<EchoZonedStatic*>(client.zoneAt(u"Rear"_s)));
}
