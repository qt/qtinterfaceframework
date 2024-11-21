// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "rotestbase.h"

using namespace Qt::StringLiterals;

class QtRoStaticBackendTest : public RoTestBase
{
    Q_OBJECT

private slots:
    void initTestCase_data() override
    {
        BackendsTestBase::initTestCase_data();
        QDir currentDir = QDir::current();

        QTest::newRow("qtro-static-backend") << "org.example.echomodule_qtro_static" << false << false << currentDir.absoluteFilePath(u"org-example-echo-qtro-server"_s + exeSuffix);
    }
};

QTEST_MAIN(QtRoStaticBackendTest)

#include "tst_qtro_backend_static.moc"
