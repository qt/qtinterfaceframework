// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtMqtt/QMqttClient>
#include <QtNetwork/QTcpSocket>
#include <QCoreApplication>
#include <QFileInfo>

#include "echomodulefactory.h"
#include "echoservice.h"
#include "echozonedservice.h"

#include "cmdlistener.h"

using namespace Qt::StringLiterals;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QMqttClient client;
    client.setHostname("localhost");
    client.setPort(1883);
    client.connectToHost();
    auto echoService = new EchoService(&client, &app);
    auto echoZonedService = new EchoZonedService(&client, &app);

    QObject::connect(&client, &QMqttClient::connected, &client,  [&app, &client, echoService, echoZonedService]() {
        qInfo() << "Connected to Broker";
        CmdListener *listener = new CmdListener(&app);

        QObject::connect(listener, &::CmdListener::newCmd, listener, [&client, echoService, echoZonedService](const QString &cmd) {
            if (cmd == u"testInit"_s) {
                QLatin1String lastMessageTestValue("this is the last message");
                int intValueTestValue(789);
                qreal floatValue1TestValue(3.14);
                qreal floatValue2TestValue(2.71);
                QLatin1String stringValueTestValue("test string");
                QList<Combo> comboListTestValue(
                    { Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday),
                     Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday) });
                Contact contactTestValue(QStringLiteral("Nemo"), 47, true, 1);
                Echomodule::WeekDay weekDayTestValue = Echomodule::Wednesday;
                Echomodule::TestEnum testEnumTestValue = Echomodule::SecondEnumValue;

                echoService->setIntValue(intValueTestValue);
                echoService->setFloatValue1(floatValue1TestValue);
                echoService->setFloatValue2(floatValue2TestValue);
                echoService->setStringValue(stringValueTestValue);
                echoService->setComboList(comboListTestValue);
                echoService->setContact(contactTestValue);
                echoService->setWeekDay(weekDayTestValue);
                echoService->setTestEnum(testEnumTestValue);
                echoService->setLastMessage(lastMessageTestValue);
            } else if (cmd == u"changeProperties"_s) {
                int intValueTestValue = 12345;
                qreal floatValue1TestValue = 1234.5678;
                qreal floatValue2TestValue = 234.678;
                QLatin1String stringValueTestValue("hello test");
                QList<Combo> comboListTestValue(
                    { Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday),
                     Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday) });
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
            } else if (cmd == u"testZonedInit"_s) {
                const QString frontLeftZone = u"FrontLeft"_s;
                QList<Combo> comboListTestValue(
                    { Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Monday),
                     Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Wednesday) });

                // The test only waits for the default-zone stringValue change before
                // checking all other values, so publish that property last to make
                // sure everything else has already been delivered.
                echoZonedService->setAirflowDirection(Echomodule::Windshield, frontLeftZone);
                echoZonedService->setContact(Contact(QStringLiteral("Nemo"), 47, true, 1), frontLeftZone);
                echoZonedService->setComboList(comboListTestValue, frontLeftZone);
                echoZonedService->setUPPERCASEPROPERTY(3.14, frontLeftZone);
                echoZonedService->setTestEnum(Echomodule::SecondEnumValue, frontLeftZone);

                echoZonedService->setIntValue(789, QString());
                echoZonedService->setVarValue(QVariant(789), QString());
                echoZonedService->setStringValue(u"test string"_s, QString());
            } else if (cmd == u"changeZoneProperties"_s) {
                const QString frontLeftZone = u"FrontLeft"_s;
                QList<Combo> comboListTestValue(
                    { Combo(Contact("Mr A.", 20, false, "foo"), Echomodule::Thursday),
                     Combo(Contact("Mr B.", 40, true, "bar"), Echomodule::Tuesday) });

                echoZonedService->setTestEnum(Echomodule::SecondEnumValue, frontLeftZone);
                echoZonedService->setAirflowDirection(Echomodule::Windshield, frontLeftZone);
                echoZonedService->setContact(Contact(QStringLiteral("Nemo"), 47, true, 1), frontLeftZone);
                echoZonedService->setComboList(comboListTestValue, frontLeftZone);
                echoZonedService->setStringValue(u"hello test"_s, frontLeftZone);
                echoZonedService->setUPPERCASEPROPERTY(1234.5678, QString());
                echoZonedService->setIntValue(12345, QString());
            } else if (cmd == u"emitSignals"_s) {
                AnotherStruct anotherTestValue(7);
                QLatin1String foobarTestValue("foo and bar");
                Q_EMIT echoService->somethingHappened();
                Q_EMIT echoService->foobar(foobarTestValue);
                Q_EMIT echoService->anotherChanged(anotherTestValue);

                echoZonedService->somethingHappened(u"FrontLeft"_s);
                echoZonedService->foobar(foobarTestValue, u"FrontLeft"_s);
                echoZonedService->anotherChanged(anotherTestValue, QString());
            } else if (cmd == u"quit"_s) {
                client.disconnectFromHost();
                qApp->quit();
            }
        });
    });


    return app.exec();
}
