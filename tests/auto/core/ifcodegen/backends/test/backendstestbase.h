// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef BACKENDSTESTBASE_H
#define BACKENDSTESTBASE_H

#include <QtTest>
#include <QLocalServer>
#include <QLocalSocket>

QT_FORWARD_DECLARE_CLASS(QIfServiceManager);

#ifdef Q_OS_WIN
static QString exeSuffix = QStringLiteral(".exe");
#else
static QString exeSuffix;
#endif

#define WAIT_AND_COMPARE(spy, value) \
if (spy.count() != value) \
        spy.wait(); \
    QCOMPARE(spy.count(), value); \
 \

class BackendsTestBase : public QObject
{
    Q_OBJECT
public:
    BackendsTestBase();

    void sendCmd(const QByteArray &input);
    void startServer(QStringList arguments = QStringList());
    void ignoreMessage(QtMsgType type, const char *message);
    void cleanupTestData();

    virtual void initTestCase_data();

private slots:
    void initTestCase();
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

protected:
#if QT_CONFIG(process)
    QProcess *m_serverProcess;
#endif
    QLocalServer *m_localServer;
    QLocalSocket *m_localSocket;
    bool m_isSimulation;
    bool m_isSimulationBackend;
    QString m_serverExecutable;
};

#endif // BACKENDSTESTBASE_H
