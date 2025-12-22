// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef ECHOSERVICE_H
#define ECHOSERVICE_H

#include <QObject>
#include <QtMqtt/QMqttClient>
#include <echomodulefactory.h>

class EchoService: public QObject
{
    Q_OBJECT

public:
    explicit EchoService(QMqttClient *client, QObject *parent = nullptr);

    Combo m_testCombo;
    QString m_testId;

    QString lastMessage() const { return m_lastMessage; }
    int intValue() const { return m_intValue; }
    QVariant varValue() const { return m_varValue; }
    bool echoEnabled() const { return m_echoEnabled; }
    qreal floatValue1() const { return m_floatValue1; }
    qreal floatValue2() const { return m_floatValue2; }
    QString stringValue() const { return m_stringValue; }
    Contact contact() const { return m_contact; }
    Combo combo() const { return m_combo; }
    QList<int> intList() const { return m_intList; }
    QList<Combo> comboList() const { return m_comboList; }
    Echomodule::DaysOfTheWeek weekDay() const { return m_weekDay; }
    Echomodule::TestEnum testEnum() const { return m_testEnum; }
    qreal UPPERCASEPROPERTY() const { return m_UPPERCASEPROPERTY; }
    OnlyAStringInAStruct stringInAStructProperty() const { return m_stringInAStructProperty; }

    void setIntValue(int intValue);
    void setVarValue(const QVariant &varValue);
    void setEchoEnabled(bool echoEnabled);
    void setFloatValue1(qreal floatValue1);
    void setFloatValue2(qreal floatValue2);
    void setStringValue(const QString &stringValue);
    void setContact(const Contact &contact);
    void setCombo(const Combo &combo);
    void setIntList(const QList<int> &intList);
    void setComboList(const QList<Combo> &comboList);
    void setWeekDay(Echomodule::DaysOfTheWeek weekDay);
    void setTestEnum(Echomodule::TestEnum testEnum);
    void setUPPERCASEPROPERTY(qreal UPPERCASEPROPERTY);
    void setStringInAStructProperty(const OnlyAStringInAStruct &stringInAStructProperty);
    void setLastMessage(const QString &lastMessage);

public Q_SLOTS:
    void echo(const QByteArray &id, const QString &msg);
    void id(const QByteArray &id);
    void getCombo(const QByteArray &id);
    void voidSlot(const QByteArray &id);
    void voidSlot2(const QByteArray &id, const QString &msg);
    void timer(const QByteArray &id, const QString &msg);
    void flagMethod(const QByteArray &id, const QString &msg);
    void enumMethod(const QByteArray &id, const QString &msg);

    void somethingHappened();
    void foobar(const QString &string);
    void anotherChanged(const AnotherStruct &anotherStruct);

private:
    QString m_lastMessage;
    int m_intValue;
    QVariant m_varValue;
    bool m_echoEnabled;
    qreal m_floatValue1;
    qreal m_floatValue2;
    QString m_stringValue;
    Contact m_contact;
    Combo m_combo;
    QList<int> m_intList;
    QList<Combo> m_comboList;
    Echomodule::DaysOfTheWeek m_weekDay;
    Echomodule::TestEnum m_testEnum;
    qreal m_UPPERCASEPROPERTY;
    OnlyAStringInAStruct m_stringInAStructProperty;

    QMqttClient *m_client;
};

#endif // ECHOSERVICE_H
