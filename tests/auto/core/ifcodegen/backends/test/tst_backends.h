// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef BACKENDSTEST_H
#define BACKENDSTEST_H

#include <QtTest>
#include <QLocalServer>
#include <QLocalSocket>

QT_FORWARD_DECLARE_CLASS(QIfServiceManager);

class BackendsTest : public QObject
{
    Q_OBJECT
public:
    BackendsTest();

    void sendCmd(const QByteArray &input);
    void startServer();
    void ignoreMessage(QtMsgType type, const char *message);
private slots:
    void initTestCase();
    void initTestCase_data();
    void init();
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
    void testSimulationData();

private:
    QProcess *m_serverProcess;
    QLocalServer *m_localServer;
    QLocalSocket *m_localSocket;
    bool m_isSimulation;
    bool m_isSimulationBackend;
    QString m_serverExecutable;
};

#endif // BACKENDSTEST_H
