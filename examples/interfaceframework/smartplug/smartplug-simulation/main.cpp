// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMqttClient>
#include <QRandomGenerator>
#include <QTimer>
#include <QUrl>
#include <QtGlobal>

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(u"smartplug-simulation"_s);
    QCoreApplication::setApplicationVersion(u"1.0"_s);

    QCommandLineParser parser;
    parser.setApplicationDescription(
        u"Simulates a smart plug on an MQTT broker for the Qt Interface Framework Smart Plug "
        "example. It reacts to the \"smartplug/on\" command and publishes simulated current, "
        "voltage and energy readings."_s);
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption urlOption({ u"u"_s, u"url"_s },
        u"The MQTT broker to connect to."_s, u"url"_s, u"mqtt://localhost:1883"_s);
    parser.addOption(urlOption);
    parser.process(app);

    const QUrl url(parser.value(urlOption));

    QMqttClient client;
    client.setHostname(url.host().isEmpty() ? u"localhost"_s : url.host());
    client.setPort(url.port(1883));

    bool on = false;
    double energy = 0.0;

    auto publish = [&client](const QString &topic, const QByteArray &value) {
        client.publish(QMqttTopicName(topic), value, /*qos*/ 0, /*retain*/ true);
    };

    auto publishOffReadings = [&publish]() {
        publish(u"smartplug/voltage"_s, "0.0");
        publish(u"smartplug/current"_s, "0.00");
    };

    QObject::connect(&client, &QMqttClient::connected, &client, [&]() {
        qInfo("smartplug-simulation: connected to %s", qUtf8Printable(url.toString()));
        client.subscribe(QMqttTopicFilter(u"smartplug/on"_s));
        // Announce the initial (retained) state so newly started apps pick it up.
        publish(u"smartplug/on"_s, on ? "true" : "false");
        if (!on)
            publishOffReadings();
        publish(u"smartplug/energy"_s, QByteArray::number(energy, 'f', 5));
    });

    QObject::connect(&client, &QMqttClient::messageReceived, &client,
                     [&](const QByteArray &message, const QMqttTopicName &topic) {
        if (topic.name() != u"smartplug/on"_s)
            return;
        const bool newState = (message == "true" || message == "1");
        if (newState == on)
            return; // ignore our own retained echo
        on = newState;
        qInfo("smartplug-simulation: switched %s", on ? "on" : "off");
        publish(u"smartplug/on"_s, on ? "true" : "false");
        if (!on)
            publishOffReadings();
    });

    // While switched on, publish changing metering values once per second.
    QTimer meter;
    meter.setInterval(1000);
    QObject::connect(&meter, &QTimer::timeout, &client, [&]() {
        if (client.state() != QMqttClient::Connected || !on)
            return;

        const double voltage = 230.0 + (QRandomGenerator::global()->bounded(20) - 10) / 10.0;
        const double current = 5.0 + QRandomGenerator::global()->bounded(100) / 100.0; // ~1.2 kW load
        energy += voltage * current / 1000.0 / 3600.0; // kWh consumed during this 1s interval

        publish(u"smartplug/voltage"_s, QByteArray::number(voltage, 'f', 1));
        publish(u"smartplug/current"_s, QByteArray::number(current, 'f', 2));
        publish(u"smartplug/energy"_s, QByteArray::number(energy, 'f', 5));
    });
    meter.start();

    client.connectToHost();

    return app.exec();
}
