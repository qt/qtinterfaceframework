// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "echoservice.h"
#include <QTimer>
#include <QJsonDocument>

EchoService::EchoService(QMqttClient* client, QObject *parent)
    : QObject(parent)
    , m_testCombo(Contact(QStringLiteral("Antti"), 34, true, QVariant()), Echomodule::Friday)
    , m_testId(QStringLiteral("id123"))
    , m_intValue(0)
    , m_varValue("value")
    , m_echoEnabled(false)
    , m_floatValue1(0)
    , m_floatValue2(0)
    , m_stringValue("test")
    , m_weekDay(Echomodule::Monday)
    , m_testEnum(Echomodule::FirstEnumValue)
    , m_UPPERCASEPROPERTY(0)
    , m_client(client)
{
    connect(client, &QMqttClient::connected, this, [this]{
        m_client->subscribe(QMqttTopicFilter("Echo/#"));

        setIntValue(m_intValue);
        setVarValue(m_varValue);
        setEchoEnabled(m_echoEnabled);
        setFloatValue1(m_floatValue1);
        setFloatValue2(m_floatValue2);
        setStringValue(m_stringValue);
        setContact(m_contact);
        setCombo(m_combo);
        setIntList(m_intList);
        setComboList(m_comboList);
        setWeekDay(m_weekDay);
        setTestEnum(m_testEnum);
        setUPPERCASEPROPERTY(m_UPPERCASEPROPERTY);
        setStringInAStructProperty(m_stringInAStructProperty);
        setLastMessage(m_lastMessage);
    });

    connect(client, &QMqttClient::errorChanged, this, [this](){
        qCritical() << "ERROR:" << m_client->error();
    });

    connect(client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        if (topic.levels().count() < 3)
            return;

        QString msgid = topic.levels().at(2);
        if (topic.name().startsWith("Echo/echo/")) {
            echo(msgid.toUtf8(), message);
        } else if (topic.name().startsWith("Echo/id/")) {
            id(msgid.toUtf8());
        } else if (topic.name().startsWith("Echo/getCombo/")) {
            getCombo(msgid.toUtf8());
        } else if (topic.name().startsWith("Echo/voidSlot/")) {
            voidSlot(msgid.toUtf8());
        } else if (topic.name().startsWith("Echo/voidSlot2/")) {
            voidSlot2(msgid.toUtf8(), message);
        } else if (topic.name().startsWith("Echo/timer/")) {
            timer(msgid.toUtf8(), message);
        } else if (topic.name().startsWith("Echo/flagMethod/")) {
            flagMethod(msgid.toUtf8(), message);
        } else if (topic.name().startsWith("Echo/enumMethod/")) {
            enumMethod(msgid.toUtf8(), message);
        }
    });

}

void EchoService::setIntValue(int intValue)
{
    m_intValue = intValue;
    m_client->publish(QMqttTopicName("Echo/intValue"), QByteArray::number(intValue));
}

void EchoService::setVarValue(const QVariant &varValue)
{
    m_varValue = varValue;
    m_client->publish(QMqttTopicName("Echo/varValue"), varValue.toByteArray());
}

void EchoService::setEchoEnabled(bool echoEnabled)
{
    m_echoEnabled = echoEnabled;
    m_client->publish(QMqttTopicName("Echo/echoEnabled"), QByteArray(echoEnabled ? "1" : "0"));
}

void EchoService::setFloatValue1(qreal floatValue1)
{
    m_floatValue1 = floatValue1;
    m_client->publish(QMqttTopicName("Echo/floatValue1"), QByteArray::number(floatValue1, 10, QLocale::FloatingPointShortest));
}

void EchoService::setFloatValue2(qreal floatValue2)
{
    m_floatValue2 = floatValue2;
    m_client->publish(QMqttTopicName("Echo/floatValue2"), QByteArray::number(floatValue2, 10, QLocale::FloatingPointShortest));
}

void EchoService::setStringValue(const QString &stringValue)
{
    m_stringValue = stringValue;
    m_client->publish(QMqttTopicName("Echo/stringValue"), stringValue.toUtf8());
}

void EchoService::setContact(const Contact &contact)
{
    m_contact = contact;
    m_client->publish(QMqttTopicName("Echo/contact"), QJsonDocument::fromVariant(contact.toVariant()).toJson(QJsonDocument::Compact));
}

void EchoService::setCombo(const Combo &combo)
{
    m_combo = combo;
    m_client->publish(QMqttTopicName("Echo/combo"), QJsonDocument::fromVariant(combo.toVariant()).toJson(QJsonDocument::Compact));
}

void EchoService::setIntList(const QList<int> &intList)
{
    m_intList = intList;

    QVariantList list;
    for (auto val : intList)
        list.append(val);
    QByteArray value = QJsonDocument::fromVariant(list).toJson(QJsonDocument::Compact);
    m_client->publish(QMqttTopicName("Echo/intList"), value);
}

void EchoService::setComboList(const QList<Combo> &comboList)
{
    m_comboList = comboList;

    QVariantList list;
    for (auto &val : comboList)
        list.append(val.toVariant());
    QByteArray value = QJsonDocument::fromVariant(list).toJson(QJsonDocument::Compact);
    m_client->publish(QMqttTopicName("Echo/comboList"), value);
}

void EchoService::setWeekDay(Echomodule::DaysOfTheWeek weekDay)
{
    m_weekDay = weekDay;
    m_client->publish(QMqttTopicName("Echo/weekDay"), QByteArray::number(int(weekDay)));
}

void EchoService::setTestEnum(Echomodule::TestEnum testEnum)
{
    m_testEnum = testEnum;
    m_client->publish(QMqttTopicName("Echo/testEnum"), QByteArray::number(int(testEnum)));
}

void EchoService::setUPPERCASEPROPERTY(qreal UPPERCASEPROPERTY)
{
    m_UPPERCASEPROPERTY = UPPERCASEPROPERTY;
    m_client->publish(QMqttTopicName("Echo/UPPERCASEPROPERTY"), QByteArray::number(UPPERCASEPROPERTY));
}

void EchoService::setStringInAStructProperty(const OnlyAStringInAStruct &stringInAStructProperty)
{
    m_stringInAStructProperty = stringInAStructProperty;
    m_client->publish(QMqttTopicName("Echo/stringInAStructProperty"),
                      QJsonDocument::fromVariant(stringInAStructProperty.toVariant()).toJson(QJsonDocument::Compact));
}

void EchoService::setLastMessage(const QString &lastMessage)
{
    m_lastMessage = lastMessage;
    m_client->publish(QMqttTopicName("Echo/lastMessage"), lastMessage.toLatin1());
}

void EchoService::echo(const QByteArray &id, const QString &msg)
{
    auto document = QJsonDocument::fromJson(msg.toUtf8());
    QString msgValue = document.toVariant().toMap().value("msg").toString();
    m_client->publish(QMqttTopicName("Echo/echo_results/" + id), msgValue.toUtf8());
}

void EchoService::id(const QByteArray &id)
{
    m_client->publish(QMqttTopicName("Echo/id_results/" + id), m_testId.toUtf8());
}

void EchoService::getCombo(const QByteArray &id)
{
    auto document = QJsonDocument::fromVariant(m_testCombo.toVariant());
    m_client->publish(QMqttTopicName("Echo/getCombo_results/" + id), document.toJson(QJsonDocument::Compact));
}

void EchoService::voidSlot(const QByteArray &id)
{
    m_client->publish(QMqttTopicName("Echo/voidSlot_results/" + id), "1");
}

void EchoService::voidSlot2(const QByteArray &id, const QString &msg)
{
    Q_UNUSED(msg);
    m_client->publish(QMqttTopicName("Echo/voidSlot2_results/" + id), "1");
}

void EchoService::timer(const QByteArray &id, const QString &msg)
{
    auto document = QJsonDocument::fromJson(msg.toUtf8());
    int interval = document.toVariant().toMap().value("interval").toInt();
    qCritical() << "start timer with interval" << interval;
    QTimer::singleShot(interval, this, [this, id](){
        qCritical() << "timer finished";
        m_client->publish(QMqttTopicName("Echo/timer_results/" + id), "1");
    });
}

void EchoService::flagMethod(const QByteArray &id, const QString &msg)
{
    auto document = QJsonDocument::fromJson(msg.toUtf8());
    QByteArray directionValue = document.toVariant().toMap().value("direction").toByteArray();
    m_client->publish(QMqttTopicName("Echo/flagMethod_results/" + id), directionValue);
}

void EchoService::enumMethod(const QByteArray &id, const QString &msg)
{
    auto document = QJsonDocument::fromJson(msg.toUtf8());
    QByteArray testEnumValue = document.toVariant().toMap().value("testEnum").toByteArray();
    m_client->publish(QMqttTopicName("Echo/enumMethod_results/" + id), testEnumValue);
}

void EchoService::somethingHappened()
{
    m_client->publish(QMqttTopicName("Echo/somethingHappened"), "1");
}

void EchoService::foobar(const QString &string)
{
    m_client->publish(QMqttTopicName("Echo/foobar"), string.toUtf8());
}

void EchoService::anotherChanged(const AnotherStruct &anotherStruct)
{
    auto document = QJsonDocument::fromVariant(anotherStruct.toVariant());
    m_client->publish(QMqttTopicName("Echo/anotherChanged"), document.toJson(QJsonDocument::Compact));
}

#include "moc_echoservice.cpp"

