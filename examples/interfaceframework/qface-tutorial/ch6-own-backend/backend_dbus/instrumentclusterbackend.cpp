// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "instrumentclusterbackend.h"

#include <QDBusConnection>

InstrumentClusterBackend::InstrumentClusterBackend(QObject *parent)
    : InstrumentClusterBackendInterface(parent)
    , m_client(nullptr)
{
    qDBusRegisterMetaType<Warning>();
    qDBusRegisterMetaType<InstrumentClusterModule::SystemType>();
}

void InstrumentClusterBackend::initialize()
{
    if (!m_client)
        setupConnection();
    if (m_fetchList.isEmpty()) {
        emit speedChanged(m_speed);
        emit rpmChanged(m_rpm);
        emit fuelChanged(m_fuel);
        emit temperatureChanged(m_temperature);
        emit systemTypeChanged(m_systemType);
        emit currentWarningChanged(m_warning);
        emit initializationDone();
    }
}

void InstrumentClusterBackend::setupConnection()
{
    qInfo() << "Connecting to the Server";
    m_client = new ExampleIfInstrumentClusterInterface("Example.If.InstrumentCluster", "/", QDBusConnection::sessionBus());
    connect(m_client, &ExampleIfInstrumentClusterInterface::speedChanged,
            this, &InstrumentClusterBackend::onSpeedChanged);
    connect(m_client, &ExampleIfInstrumentClusterInterface::rpmChanged,
            this, &InstrumentClusterBackend::onRpmChanged);
    connect(m_client, &ExampleIfInstrumentClusterInterface::fuelChanged,
            this, &InstrumentClusterBackend::onFuelChanged);
    connect(m_client, &ExampleIfInstrumentClusterInterface::temperatureChanged,
            this, &InstrumentClusterBackend::onTemperaturChanged);
    connect(m_client, &ExampleIfInstrumentClusterInterface::systemTypeChanged,
            this, &InstrumentClusterBackend::onSystemTypeChanged);
    connect(m_client, &ExampleIfInstrumentClusterInterface::currentWarningChanged,
            this, &InstrumentClusterBackend::onCurrentWarningChanged);
    fetchSpeed();
    fetchRpm();
    fetchFuel();
    fetchTemperature();
    fetchSystemType();
    fetchCurrentWarning();
}

void InstrumentClusterBackend::fetchSpeed()
{
    m_fetchList.append("speed");
    auto reply = m_client->asyncCall("speed");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<int> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("speed");
            this->onSpeedChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchRpm()
{
    m_fetchList.append("rpm");
    auto reply = m_client->asyncCall("rpm");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<int> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("rpm");
            this->onRpmChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchFuel()
{
    m_fetchList.append("fuel");
    auto reply = m_client->asyncCall("fuel");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<qreal> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("fuel");
            this->onFuelChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchTemperature()
{
    m_fetchList.append("temperature");
    auto reply = m_client->asyncCall("temperature");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<qreal> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("temperature");
            this->temperatureChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchSystemType()
{
    m_fetchList.append("systemType");
    auto reply = m_client->asyncCall("systemType");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<InstrumentClusterModule::SystemType> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("systemType");
            this->onSystemTypeChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::fetchCurrentWarning()
{
    m_fetchList.append("currentWarning");
    auto reply = m_client->asyncCall("currentWarning");
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<Warning> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll("currentWarning");
            this->onCurrentWarningChanged(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}

void InstrumentClusterBackend::checkInitDone()
{
    if (m_fetchList.isEmpty()) {
        qInfo() << "All properties initialized";
        emit initializationDone();
    }
}

void InstrumentClusterBackend::onSpeedChanged(int speed)
{
    m_speed = speed;
    emit speedChanged(speed);
}

void InstrumentClusterBackend::onRpmChanged(int rpm)
{
    m_rpm = rpm;
    emit rpmChanged(rpm);
}

void InstrumentClusterBackend::onFuelChanged(qreal fuel)
{
    m_fuel = fuel;
    emit fuelChanged(fuel);
}

void InstrumentClusterBackend::onTemperaturChanged(qreal temperature)
{
    m_temperature = temperature;
    emit temperatureChanged(temperature);
}

void InstrumentClusterBackend::onSystemTypeChanged(InstrumentClusterModule::SystemType systemType)
{
    m_systemType = systemType;
    emit systemTypeChanged(systemType);
}

void InstrumentClusterBackend::onCurrentWarningChanged(const Warning &warning)
{
    m_warning = warning;
    emit currentWarningChanged(warning);
}
