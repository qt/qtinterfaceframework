// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "tst_echoqtro.h"
#include <QTest>

int main(int argc, char *argv[])
{
    EchoQtroTest echoTest;
    QCoreApplication app(argc, argv);

    return QTest::qExec(&echoTest, argc, argv);
}
