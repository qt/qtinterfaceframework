// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "echozonedservice.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

using namespace Qt::StringLiterals;

EchoZonedService::EchoZonedService(QMqttClient *client, QObject *parent)
    : QObject(parent)
    , m_testCombo(Contact(QStringLiteral("Antti"), 34, true, QVariant()), Echomodule::Friday)
    , m_testId(QStringLiteral("id123"))
    , m_client(client)
{
    // The default (empty) zone always exists, "FrontLeft" is the only named zone.
    m_zones.insert(QString(), ZoneData());
    m_zones.insert(u"FrontLeft"_s, ZoneData());

    connect(client, &QMqttClient::connected, this, [this]{
        // The EchoZoned interface uses its own topic prefix to stay isolated from
        // the (non-zoned) Echo interface, which shares the same broker.
        m_client->subscribe(QMqttTopicFilter(u"EchoZoned/#"_s));

        // Announce the available zones. Retained so that the value is also
        // delivered to clients connecting later.
        QJsonArray zonesArray;
        zonesArray.append(u"FrontLeft"_s);
        m_client->publish(QMqttTopicName(u"EchoZoned/availableZones"_s),
                          QJsonDocument(zonesArray).toJson(QJsonDocument::Compact), 0, true);

        // Publish the initial value of every property for every zone, so the
        // client can finish synchronizing all (mandatory) properties.
        for (auto it = m_zones.constBegin(); it != m_zones.constEnd(); ++it)
            publishAll(it.key());
    });

    connect(client, &QMqttClient::errorChanged, this, [this](){
        qCritical() << "ERROR:" << m_client->error();
    });

    connect(client, &QMqttClient::messageReceived, this, [this](const QByteArray &message, const QMqttTopicName &topic) {
        if (topic.levels().count() < 3)
            return;

        const QByteArray msgId = topic.levels().constLast().toUtf8();
        const QString name = topic.name();
        const QVariantMap arguments = QJsonDocument::fromJson(message).toVariant().toMap();

        if (name.startsWith(u"EchoZoned/echo/"_s)) {
            echo(msgId, arguments.value(u"msg"_s).toString());
        } else if (name.startsWith(u"EchoZoned/id/"_s)) {
            id(msgId);
        } else if (name.startsWith(u"EchoZoned/getCombo/"_s)) {
            getCombo(msgId);
        } else if (name.startsWith(u"EchoZoned/timer/"_s)) {
            timer(msgId, arguments.value(u"interval"_s).toInt(), arguments.value(u"zone"_s).toString());
        } else if (name.startsWith(u"EchoZoned/flagMethod/"_s)) {
            flagMethod(msgId, arguments.value(u"direction"_s).toByteArray());
        } else if (name.startsWith(u"EchoZoned/enumMethod/"_s)) {
            enumMethod(msgId, arguments.value(u"testEnum"_s).toByteArray());
        }
    });
}

QString EchoZonedService::topicFor(const QString &zone, const QString &symbol) const
{
    if (zone.isEmpty())
        return u"EchoZoned/"_s + symbol;
    return u"EchoZoned/"_s + zone + u"/"_s + symbol;
}

void EchoZonedService::publishAll(const QString &zone)
{
    const ZoneData &d = m_zones[zone];

    m_client->publish(QMqttTopicName(topicFor(zone, u"lastMessage"_s)), d.lastMessage.toUtf8());
    m_client->publish(QMqttTopicName(topicFor(zone, u"intValue"_s)), QByteArray::number(d.intValue));
    m_client->publish(QMqttTopicName(topicFor(zone, u"varValue"_s)), d.varValue.toByteArray());
    m_client->publish(QMqttTopicName(topicFor(zone, u"stringValue"_s)), d.stringValue.toUtf8());
    m_client->publish(QMqttTopicName(topicFor(zone, u"unsupportedValue"_s)), d.unsupportedValue.toUtf8());
    m_client->publish(QMqttTopicName(topicFor(zone, u"zonedValue"_s)), QByteArray(d.zonedValue ? "1" : "0"));
    m_client->publish(QMqttTopicName(topicFor(zone, u"valueWithDefault"_s)), QByteArray(d.valueWithDefault ? "1" : "0"));
    m_client->publish(QMqttTopicName(topicFor(zone, u"echoEnabled"_s)), QByteArray(d.echoEnabled ? "1" : "0"));
    m_client->publish(QMqttTopicName(topicFor(zone, u"airflowDirection"_s)), QByteArray::number(int(d.airflowDirection)));
    m_client->publish(QMqttTopicName(topicFor(zone, u"rangedValue"_s)), QByteArray::number(d.rangedValue));
    m_client->publish(QMqttTopicName(topicFor(zone, u"rangedValueWithDefault"_s)), QByteArray::number(d.rangedValueWithDefault));
    m_client->publish(QMqttTopicName(topicFor(zone, u"contact"_s)), QJsonDocument::fromVariant(d.contact.toVariant()).toJson(QJsonDocument::Compact));
    m_client->publish(QMqttTopicName(topicFor(zone, u"combo"_s)), QJsonDocument::fromVariant(d.combo.toVariant()).toJson(QJsonDocument::Compact));

    QVariantList intList;
    for (int val : d.intList)
        intList.append(val);
    m_client->publish(QMqttTopicName(topicFor(zone, u"intList"_s)), QJsonDocument::fromVariant(intList).toJson(QJsonDocument::Compact));

    QVariantList comboList;
    for (const auto &val : d.comboList)
        comboList.append(val.toVariant());
    m_client->publish(QMqttTopicName(topicFor(zone, u"comboList"_s)), QJsonDocument::fromVariant(comboList).toJson(QJsonDocument::Compact));

    m_client->publish(QMqttTopicName(topicFor(zone, u"weekDay"_s)), QByteArray::number(int(d.weekDay)));
    m_client->publish(QMqttTopicName(topicFor(zone, u"testEnum"_s)), QByteArray::number(int(d.testEnum)));
    m_client->publish(QMqttTopicName(topicFor(zone, u"UPPERCASEPROPERTY"_s)), QByteArray::number(d.UPPERCASEPROPERTY, 'g', 17));
}

void EchoZonedService::setIntValue(int intValue, const QString &zone)
{
    m_zones[zone].intValue = intValue;
    m_client->publish(QMqttTopicName(topicFor(zone, u"intValue"_s)), QByteArray::number(intValue));
}

void EchoZonedService::setVarValue(const QVariant &varValue, const QString &zone)
{
    m_zones[zone].varValue = varValue;
    m_client->publish(QMqttTopicName(topicFor(zone, u"varValue"_s)), varValue.toByteArray());
}

void EchoZonedService::setStringValue(const QString &stringValue, const QString &zone)
{
    m_zones[zone].stringValue = stringValue;
    m_client->publish(QMqttTopicName(topicFor(zone, u"stringValue"_s)), stringValue.toUtf8());
}

void EchoZonedService::setContact(const Contact &contact, const QString &zone)
{
    m_zones[zone].contact = contact;
    m_client->publish(QMqttTopicName(topicFor(zone, u"contact"_s)),
                      QJsonDocument::fromVariant(contact.toVariant()).toJson(QJsonDocument::Compact));
}

void EchoZonedService::setComboList(const QList<Combo> &comboList, const QString &zone)
{
    m_zones[zone].comboList = comboList;

    QVariantList list;
    for (const auto &val : comboList)
        list.append(val.toVariant());
    m_client->publish(QMqttTopicName(topicFor(zone, u"comboList"_s)),
                      QJsonDocument::fromVariant(list).toJson(QJsonDocument::Compact));
}

void EchoZonedService::setAirflowDirection(Echomodule::AirflowDirections airflowDirection, const QString &zone)
{
    m_zones[zone].airflowDirection = airflowDirection;
    m_client->publish(QMqttTopicName(topicFor(zone, u"airflowDirection"_s)), QByteArray::number(int(airflowDirection)));
}

void EchoZonedService::setTestEnum(Echomodule::TestEnum testEnum, const QString &zone)
{
    m_zones[zone].testEnum = testEnum;
    m_client->publish(QMqttTopicName(topicFor(zone, u"testEnum"_s)), QByteArray::number(int(testEnum)));
}

void EchoZonedService::setUPPERCASEPROPERTY(qreal UPPERCASEPROPERTY, const QString &zone)
{
    m_zones[zone].UPPERCASEPROPERTY = UPPERCASEPROPERTY;
    m_client->publish(QMqttTopicName(topicFor(zone, u"UPPERCASEPROPERTY"_s)), QByteArray::number(UPPERCASEPROPERTY, 'g', 17));
}

void EchoZonedService::somethingHappened(const QString &zone)
{
    m_client->publish(QMqttTopicName(topicFor(zone, u"somethingHappened"_s)), "1");
}

void EchoZonedService::foobar(const QString &string, const QString &zone)
{
    m_client->publish(QMqttTopicName(topicFor(zone, u"foobar"_s)), string.toUtf8());
}

void EchoZonedService::anotherChanged(const AnotherStruct &anotherStruct, const QString &zone)
{
    m_client->publish(QMqttTopicName(topicFor(zone, u"anotherChanged"_s)),
                      QJsonDocument::fromVariant(anotherStruct.toVariant()).toJson(QJsonDocument::Compact));
}

void EchoZonedService::echo(const QByteArray &id, const QString &msg)
{
    m_client->publish(QMqttTopicName("EchoZoned/echo_results/" + id), msg.toUtf8());
}

void EchoZonedService::id(const QByteArray &id)
{
    m_client->publish(QMqttTopicName("EchoZoned/id_results/" + id), m_testId.toUtf8());
}

void EchoZonedService::getCombo(const QByteArray &id)
{
    auto document = QJsonDocument::fromVariant(m_testCombo.toVariant());
    m_client->publish(QMqttTopicName("EchoZoned/getCombo_results/" + id), document.toJson(QJsonDocument::Compact));
}

void EchoZonedService::timer(const QByteArray &id, int interval, const QString &zone)
{
    // The zoned timer() operation returns the zone it was called for.
    QTimer::singleShot(interval, this, [this, id, zone](){
        m_client->publish(QMqttTopicName("EchoZoned/timer_results/" + id), zone.toUtf8());
    });
}

void EchoZonedService::flagMethod(const QByteArray &id, const QByteArray &direction)
{
    m_client->publish(QMqttTopicName("EchoZoned/flagMethod_results/" + id), direction);
}

void EchoZonedService::enumMethod(const QByteArray &id, const QByteArray &testEnum)
{
    m_client->publish(QMqttTopicName("EchoZoned/enumMethod_results/" + id), testEnum);
}

#include "moc_echozonedservice.cpp"
