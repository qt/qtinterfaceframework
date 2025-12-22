// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "cmdlistener.h"
#include <QLocalSocket>

CmdListener::CmdListener(QObject *parent)
    : QObject{parent}
{
    auto socket = new QLocalSocket(this);
    socket->connectToServer("qifcmdsocket");
    connect(socket, &QIODevice::readyRead, this, [this, socket]() {
        while (socket->canReadLine()) {
            QString cmd = QString::fromLocal8Bit(socket->readLine()).chopped(1);
            //qDebug() << "NEW CMD " << cmd;
            emit newCmd(cmd);
        }
    });
    m_reconnectingConnection = connect(socket, &QLocalSocket::disconnected, this, [socket]() {
        socket->connectToServer("qifcmdsocket");
    });
}

CmdListener::~CmdListener()
{
    disconnect(m_reconnectingConnection);
}

#include "moc_cmdlistener.cpp"
