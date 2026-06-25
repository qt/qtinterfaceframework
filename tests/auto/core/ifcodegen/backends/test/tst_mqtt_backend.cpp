// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "backendstestbase.h"

using namespace Qt::StringLiterals;

class MqttBackendTest : public BackendsTestBase
{
    Q_OBJECT

public:
    MqttBackendTest()
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

    bool checkForRunningBroker()
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

    ~MqttBackendTest()
    {
        m_brokerProcess->kill();
        m_brokerProcess->waitForFinished();
    }

private slots:
    void initTestCase_data() override
    {
        BackendsTestBase::initTestCase_data();
        QDir currentDir = QDir::current();

        QTest::newRow("mqtt-backend") << "*echo_backend_mqtt*" << false << false <<  currentDir.absoluteFilePath(u"org-example-echo-mqtt-server"_s + exeSuffix);
        QTest::newRow("mqtt-backend asyncBackendLoading") << "*echo_backend_mqtt*" << false << true << currentDir.absoluteFilePath(u"org-example-echo-mqtt-server"_s + exeSuffix);
    }

private:
    QProcess *m_brokerProcess;
};

QTEST_MAIN(MqttBackendTest)

#include "tst_mqtt_backend.moc"
