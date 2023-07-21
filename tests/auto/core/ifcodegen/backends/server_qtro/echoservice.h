// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H

#include "rep_echo_source.h"

class EchoService: public EchoSimpleSource
{
    Q_OBJECT

public:
    explicit EchoService(QObject *parent = nullptr);

    Combo m_testCombo;
    QString m_testId;

    virtual void setLastMessage(QString lastMessage) override;

public Q_SLOTS:
    QVariant echo(const QString &msg) override;
    QVariant id() override;
    QVariant getCombo() override;
    QVariant voidSlot() override;
    QVariant voidSlot2(int param) override;
    QVariant timer(int interval) override;
    QVariant flagMethod(Echomodule::AirflowDirections direction) override;
    QVariant enumMethod(Echomodule::TestEnum testEnum) override;

Q_SIGNALS:
    void echoSlotCalled(const QString &msg);
    void idSlotCalled();
    void getComboSlotCalled();
    void voidSlotCalled();
    void voidSlot2Called(int param);
    void flagMethodCalled(Echomodule::AirflowDirections direction);
    void enumMethodCalled(Echomodule::TestEnum testEnum);
};

#endif // ECHOSERVICE_H
