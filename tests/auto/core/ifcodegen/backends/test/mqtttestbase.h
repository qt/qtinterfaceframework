// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef MQTTTESTBASE_H
#define MQTTTESTBASE_H

#include "backendstestbase.h"

QT_FORWARD_DECLARE_CLASS(QProcess)

class MqttTestBase : public BackendsTestBase
{
    Q_OBJECT

public:
    MqttTestBase();
    ~MqttTestBase() override;

    bool checkForRunningBroker();

private slots:
    // Verifies a zoned interface whose zones are configured statically via the
    // 'zones' annotation, instead of being discovered over MQTT. The properties
    // are optional, so the backend initializes without a server.
    void testStaticZones();

private:
    QProcess *m_brokerProcess;
};

#endif // MQTTTESTBASE_H
