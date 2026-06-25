// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "mqtttestbase.h"

using namespace Qt::StringLiterals;

class MqttBackendTest : public MqttTestBase
{
    Q_OBJECT

private slots:
    void initTestCase_data() override
    {
        BackendsTestBase::initTestCase_data();
        QDir currentDir = QDir::current();

        QTest::newRow("mqtt-backend") << "*echo_backend_mqtt*" << false << false <<  currentDir.absoluteFilePath(u"org-example-echo-mqtt-server"_s + exeSuffix);
        QTest::newRow("mqtt-backend asyncBackendLoading") << "*echo_backend_mqtt*" << false << true << currentDir.absoluteFilePath(u"org-example-echo-mqtt-server"_s + exeSuffix);
    }
};

QTEST_MAIN(MqttBackendTest)

#include "tst_mqtt_backend.moc"
