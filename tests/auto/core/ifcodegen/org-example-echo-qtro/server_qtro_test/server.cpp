// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "server.h"
#include "core.h"
#include <QTest>

bool Server::start()
{
    bool val =  true;
    val = val && Core::instance()->host()->enableRemoting(&m_echoService, QStringLiteral("org.example.echomodule.Echo"));
    val = val && Core::instance()->host()->enableRemoting(&m_contactsModelService, QStringLiteral("org.example.echomodule.Echo.contactList"));
    val = val && Core::instance()->host()->enableRemoting(&m_echoZonedService, QStringLiteral("org.example.echomodule.EchoZoned"));
    //Give QtRO time to announce the service
    QTest::qWait(200);
    return val;
}

bool Server::stop()
{
    bool val =  true;
    val = val && Core::instance()->host()->disableRemoting(&m_echoService);
    val = val && Core::instance()->host()->disableRemoting(&m_contactsModelService);
    val = val && Core::instance()->host()->disableRemoting(&m_echoZonedService);
    //Give QtRO time to send the disconnect message to the Replica
    QTest::qWait(200);
    return  val;
}

Server::~Server()
{
    stop();
}
