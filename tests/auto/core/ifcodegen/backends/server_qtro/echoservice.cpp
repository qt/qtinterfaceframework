// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "echoservice.h"
#include <QtIfRemoteObjectsHelper/private/qifremoteobjectspendingresult_p.h>
#include <QTimer>

EchoService::EchoService()
    : m_testCombo(Contact(QStringLiteral("Antti"), 34, true, QVariant()), Echomodule::Friday)
    , m_testId(QStringLiteral("id123"))
{
    setTestEnum(Echomodule::FirstEnumValue);
}

void EchoService::setLastMessage(QString lastMessage)
{
    EchoSimpleSource::setLastMessage(lastMessage);
}

QVariant EchoService::echo(const QString &msg)
{
    emit echoSlotCalled(msg);
    return msg;
}

QVariant EchoService::id()
{
    emit idSlotCalled();
    return m_testId;
}

QVariant EchoService::getCombo()
{
    emit getComboSlotCalled();
    return QVariant::fromValue(m_testCombo);
}

QVariant EchoService::voidSlot()
{
    emit voidSlotCalled();
    return QVariant();
}

QVariant EchoService::voidSlot2(int param)
{
    emit voidSlot2Called(param);
    return QVariant();
}

QVariant EchoService::timer(int interval)
{
    static quint64 counter = 0;
    QIfRemoteObjectsPendingResult pendingResult(counter++, false);
    QTimer::singleShot(interval, this, [this, pendingResult](){
        emit pendingResultAvailable(pendingResult.id(), true, QVariant());
    });
    return QVariant::fromValue(pendingResult);
}

QVariant EchoService::flagMethod(Echomodule::AirflowDirections direction)
{
    emit flagMethodCalled(direction);
    return QVariant::fromValue(direction);
}

QVariant EchoService::enumMethod(Echomodule::TestEnum testEnum)
{
    emit enumMethodCalled(testEnum);
    return QVariant::fromValue(testEnum);
}

#include "moc_echoservice.cpp"
