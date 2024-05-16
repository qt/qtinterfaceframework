// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "backendstestbase.h"

class SimulationBackendStaticTest : public BackendsTestBase
{
    Q_OBJECT

private slots:
    void initTestCase_data() override
    {
        BackendsTestBase::initTestCase_data();

        QTest::newRow("simulation-static-backend") << "org.example.echomodule_simulator_static" << true << false << "";
    }
};

QTEST_MAIN(SimulationBackendStaticTest)

#include "tst_simulation_backend_static.moc"
