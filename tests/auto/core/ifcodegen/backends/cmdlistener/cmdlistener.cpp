// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "cmdlistener.h"
#include <QLocalSocket>

CmdListener::CmdListener(QObject *parent)
    : QObject{parent}
{
    auto socket = new QLocalSocket(this);
    socket->connectToServer("qifcmdsocket");
    connect(socket, &QIODevice::readyRead, [this, socket]() {
        while (socket->canReadLine()) {
            QString cmd = QString::fromLocal8Bit(socket->readLine()).chopped(1);
            //qDebug() << "NEW CMD " << cmd;
            emit newCmd(cmd);
        }
    });
    connect(socket, &QLocalSocket::disconnected, [socket]() {
        socket->connectToServer("qifcmdsocket");
    });
}

#include "moc_cmdlistener.cpp"
