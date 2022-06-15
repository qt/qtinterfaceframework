// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef SERVER_H
#define SERVER_H

#include "echoservice.h"
#include "echozonedservice.h"
#include "contactsmodelservice.h"

class Server : public QObject
{
    Q_OBJECT

public Q_SLOTS:
    bool start();
    bool stop();

public:
    EchoService m_echoService;
    EchoZonedService m_echoZonedService;
    ContactsModelService m_contactsModelService;

    ~Server();

};

#endif // SERVER_H
