// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "rotestbase.h"

using namespace Qt::StringLiterals;

class QtRoBackendTest : public RoTestBase
{
    Q_OBJECT

private slots:
    void initTestCase_data() override
    {
        BackendsTestBase::initTestCase_data();
        QDir currentDir = QDir::current();

        QTest::newRow("qtro-server") << "*echo_backend_qtro*" << false << false << currentDir.absoluteFilePath(u"org-example-echo-qtro-server"_s + exeSuffix);
        QTest::newRow("qtro-server asyncBackendLoading") << "*echo_backend_qtro*" << false << true << currentDir.absoluteFilePath(u"org-example-echo-qtro-server"_s + exeSuffix);
        QTest::newRow("qtro-simulation-server") << "*echo_backend_qtro*" << true << false << currentDir.absoluteFilePath(u"org-example-echo-qtro-simulation-server"_s + exeSuffix);
    }
};

QTEST_MAIN(QtRoBackendTest)

#include "tst_qtro_backend.moc"
