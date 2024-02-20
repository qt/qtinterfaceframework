// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QIfRemoteObjectsConfig>

#include "echoservice.h"
#include "echozonedservice.h"
#include "contactsmodelservice.h"

#include "cmdlistener.h"

using namespace Qt::StringLiterals;

static QString frontLeftZone = u"FrontLeft"_s;

void serverMain(QIfRemoteObjectsConfig &config)
{
    EchoService *echoService = new EchoService(qApp);
    EchoZonedService *echoZonedService = new EchoZonedService(qApp);
    ContactsModelService *contactsModelService = new ContactsModelService(qApp);

    config.enableRemoting(u"org.example.echomodule"_s, u"Echo"_s, echoService);
    config.enableRemoting(u"org.example.echomodule"_s, u"EchoZoned"_s, echoZonedService);
    config.enableRemoting(u"org.example.echomodule"_s, u"Echo.contactList"_s, contactsModelService);

    CmdListener *listener = new CmdListener;
    QObject::connect(listener, &::CmdListener::newCmd, listener, [echoService, echoZonedService, contactsModelService](const QString &cmd) {
        if (cmd == u"testInit"_s) {
            QLatin1String lastMessageTestValue("this is the last message");
            int intValueTestValue(789);
            qreal floatValue1TestValue(3.14);
            qreal floatValue2TestValue(2.71);
            QLatin1String stringValueTestValue("test string");
            QVariantList comboListTestValue(
                { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
                 QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
            Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
            Echomodule::WeekDay weekDayTestValue = Echomodule::Wednesday;
            Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;

            echoService->setLastMessage(lastMessageTestValue);
            echoService->setIntValue(intValueTestValue);
            echoService->setFloatValue1(floatValue1TestValue);
            echoService->setFloatValue2(floatValue2TestValue);
            echoService->setStringValue(stringValueTestValue);
            echoService->setComboList(comboListTestValue);
            echoService->setContact(contactTestValue);
            echoService->setWeekDay(weekDayTestValue);
            echoService->setTestEnum(testEnumTestValue);
        } else if (cmd == u"testZonedInit"_s) {
            int intValueTestValue(789);
            QVariant varValueTestValue(789);
            qreal floatValueTestValue(3.14);
            QLatin1String stringValueTestValue("test string");
            QVariantList comboListTestValue(
                { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
                 QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
            Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
            Echomodule::AirflowDirections airflowTestValue = Echomodule::Windshield;
            Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;

            echoZonedService->setIntValue(intValueTestValue, QString());
            echoZonedService->setVarValue(varValueTestValue, QString());
            echoZonedService->setStringValue(stringValueTestValue, QString());

            echoZonedService->setAirflowDirection(airflowTestValue, frontLeftZone);
            echoZonedService->setContact(contactTestValue, frontLeftZone);
            echoZonedService->setComboList(comboListTestValue, frontLeftZone);
            echoZonedService->setUPPERCASEPROPERTY(floatValueTestValue, frontLeftZone);
            echoZonedService->setTestEnum(testEnumTestValue, frontLeftZone);
        } else if (cmd == u"changeProperties"_s) {
            int intValueTestValue = 12345;
            qreal floatValue1TestValue = 1234.5678;
            qreal floatValue2TestValue = 234.678;
            QLatin1String stringValueTestValue("hello test");
            QVariantList comboListTestValue(
                { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday)),
                 QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday)) });
            Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
            Echomodule::WeekDay weekDayTestValue = Echomodule::Friday;
            Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;
            echoService->setIntValue(intValueTestValue);
            echoService->setFloatValue1(floatValue1TestValue);
            echoService->setFloatValue2(floatValue2TestValue);
            echoService->setStringValue(stringValueTestValue);
            echoService->setComboList(comboListTestValue);
            echoService->setContact(contactTestValue);
            echoService->setWeekDay(weekDayTestValue);
            echoService->setTestEnum(testEnumTestValue);
        } else if (cmd == u"changeZoneProperties"_s) {
            int intValueTestValue = 12345;
            qreal floatValueTestValue = 1234.5678;
            QString stringValueTestValue = QStringLiteral("hello test");
            QVariantList comboListTestValue(
                { QVariant::fromValue<Combo>(Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Thursday)),
                 QVariant::fromValue<Combo>(Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Tuesday)) });
            Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
            Echomodule::AirflowDirections airflowTestValue = Echomodule::Windshield;
            Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;
            echoZonedService->setTestEnum(testEnumTestValue, frontLeftZone);
            echoZonedService->setAirflowDirection(airflowTestValue, frontLeftZone);
            echoZonedService->setContact(contactTestValue, frontLeftZone);
            echoZonedService->setComboList(comboListTestValue, frontLeftZone);
            echoZonedService->setStringValue(stringValueTestValue, frontLeftZone);
            echoZonedService->setUPPERCASEPROPERTY(floatValueTestValue, QString());
            echoZonedService->setIntValue(intValueTestValue, QString());
        } else if (cmd == u"emitSignals"_s) {
            AnotherStruct anotherTestValue(7);
            QLatin1String foobarTestValue("foo and bar");
            Q_EMIT echoZonedService->somethingHappened(frontLeftZone);
            Q_EMIT echoZonedService->foobar(foobarTestValue, frontLeftZone);
            Q_EMIT echoZonedService->anotherChanged(anotherTestValue, QString());
            Q_EMIT echoService->somethingHappened();
            Q_EMIT echoService->foobar(foobarTestValue);
            Q_EMIT echoService->anotherChanged(anotherTestValue);
        } else if (cmd == u"insert"_s) {
            Contact testContact(QStringLiteral("Mr A."), 20, false, "foo");
            contactsModelService->insert(0, testContact);
        } else if (cmd == u"update"_s) {
            Contact updatedContact(QStringLiteral("Mr B."), 30, true, QVariant());
            contactsModelService->update(0, updatedContact);
        } else if (cmd == u"remove"_s) {
            contactsModelService->remove(0);
        } else if (cmd == u"quit"_s) {
            qApp->quit();
        }
    });
}
