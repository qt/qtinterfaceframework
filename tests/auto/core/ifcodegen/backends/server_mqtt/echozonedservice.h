// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef ECHOZONEDSERVICE_H
#define ECHOZONEDSERVICE_H

#include <QHash>
#include <QObject>
#include <QtMqtt/QMqttClient>
#include <echomodulefactory.h>

class EchoZonedService : public QObject
{
    Q_OBJECT

public:
    explicit EchoZonedService(QMqttClient *client, QObject *parent = nullptr);

    Combo m_testCombo;
    QString m_testId;

    // Property setters. They update the per-zone state and publish the new value.
    void setIntValue(int intValue, const QString &zone);
    void setVarValue(const QVariant &varValue, const QString &zone);
    void setStringValue(const QString &stringValue, const QString &zone);
    void setContact(const Contact &contact, const QString &zone);
    void setComboList(const QList<Combo> &comboList, const QString &zone);
    void setAirflowDirection(Echomodule::AirflowDirections airflowDirection, const QString &zone);
    void setTestEnum(Echomodule::TestEnum testEnum, const QString &zone);
    void setUPPERCASEPROPERTY(qreal UPPERCASEPROPERTY, const QString &zone);

    // "Signals". These are plain methods publishing to the zone topic, mirroring
    // the way the non-zoned EchoService publishes its signals.
    void somethingHappened(const QString &zone);
    void foobar(const QString &string, const QString &zone);
    void anotherChanged(const AnotherStruct &anotherStruct, const QString &zone);

private:
    struct ZoneData {
        QString lastMessage;
        int intValue = 0;
        QVariant varValue;
        QString stringValue;
        QString unsupportedValue;
        bool zonedValue = false;
        bool valueWithDefault = false;
        bool echoEnabled = false;
        Echomodule::AirflowDirections airflowDirection;
        int rangedValue = 0;
        int rangedValueWithDefault = 0;
        Contact contact;
        Combo combo;
        QList<int> intList;
        QList<Combo> comboList;
        Echomodule::DaysOfTheWeek weekDay = Echomodule::DaysOfTheWeek(0);
        Echomodule::TestEnum testEnum = Echomodule::FirstEnumValue;
        qreal UPPERCASEPROPERTY = 0.0;
    };

    QString topicFor(const QString &zone, const QString &symbol) const;
    void publishAll(const QString &zone);

    // Operation handlers
    void echo(const QByteArray &id, const QString &msg);
    void id(const QByteArray &id);
    void getCombo(const QByteArray &id);
    void timer(const QByteArray &id, int interval, const QString &zone);
    void flagMethod(const QByteArray &id, const QByteArray &direction);
    void enumMethod(const QByteArray &id, const QByteArray &testEnum);

    QHash<QString, ZoneData> m_zones;
    QMqttClient *m_client;
};

#endif // ECHOZONEDSERVICE_H
