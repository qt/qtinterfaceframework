// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef ECHOQTROTEST_H
#define ECHOQTROTEST_H

#include <QtTest>

QT_FORWARD_DECLARE_CLASS(QIfServiceManager);

class EchoQtroTest : public QObject
{
    Q_OBJECT
public:
    EchoQtroTest();

private slots:
    void cleanup();
    void testInit();
    void testZonedInit();
    void testReconnect();
    void testClient2Server();
    void testZonedClient2Server();
    void testServer2Client();
    void testZonedServer2Client();
    void testSlots();
    void testZonedSlots();
    void testMultipleSlotCalls();
    void testAsyncSlotResults();
    void testSignals();
    void testModel();
};

#endif // ECHOQTROTEST_H
