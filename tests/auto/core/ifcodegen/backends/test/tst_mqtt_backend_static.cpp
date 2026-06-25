// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "mqtttestbase.h"

using namespace Qt::StringLiterals;

class MqttStaticBackendTest : public MqttTestBase
{
    Q_OBJECT

private slots:
    void initTestCase_data() override
    {
        BackendsTestBase::initTestCase_data();
        QDir currentDir = QDir::current();

        QTest::newRow("mqtt-static-backend") << "org.example.echomodule_mqtt_static" << false << false << currentDir.absoluteFilePath(u"org-example-echo-mqtt-server"_s + exeSuffix);
    }
};

QTEST_MAIN(MqttStaticBackendTest)

#include "tst_mqtt_backend_static.moc"
