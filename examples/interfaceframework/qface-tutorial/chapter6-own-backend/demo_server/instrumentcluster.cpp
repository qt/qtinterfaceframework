// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "instrumentcluster.h"
#include <QThread>

InstrumentCluster::InstrumentCluster(QObject *parent)
    : QObject(parent)
    , m_speed(0)
    , m_rpm(0)
    , m_fuel(0)
    , m_temperature(15)
    , m_systemType(InstrumentClusterModule::Metric)
{
    startTimer(100);
}

int InstrumentCluster::speed() const
{
    //QThread::sleep(5);
    return m_speed;
}

int InstrumentCluster::rpm() const
{
    return m_rpm;
}

qreal InstrumentCluster::fuel() const
{
    return m_fuel;
}

qreal InstrumentCluster::temperature() const
{
    return m_temperature;
}

InstrumentClusterModule::SystemType InstrumentCluster::systemType() const
{
    return m_systemType;
}

Warning InstrumentCluster::currentWarning() const
{
    return m_currentWarning;
}

void InstrumentCluster::setSpeed(int speed)
{
    if (m_speed == speed)
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}

void InstrumentCluster::setRpm(int rpm)
{
    if (m_rpm == rpm)
        return;

    m_rpm = rpm;
    emit rpmChanged(m_rpm);
}

void InstrumentCluster::setFuel(qreal fuel)
{
    if (qFuzzyCompare(m_fuel, fuel))
        return;

    m_fuel = fuel;
    emit fuelChanged(m_fuel);
}

void InstrumentCluster::setTemperature(qreal temperature)
{
    if (qFuzzyCompare(m_temperature, temperature))
        return;

    m_temperature = temperature;
    emit temperatureChanged(m_temperature);
}

void InstrumentCluster::setSystemType(InstrumentClusterModule::SystemType systemType)
{
    if (m_systemType == systemType)
        return;

    m_systemType = systemType;
    emit systemTypeChanged(m_systemType);
}

void InstrumentCluster::setCurrentWarning(const Warning &currentWarning)
{
    if (m_currentWarning == currentWarning)
        return;

    m_currentWarning = currentWarning;
    emit currentWarningChanged(m_currentWarning);
}

void InstrumentCluster::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (speed() >= 250)
        setSpeed(0);
    else
        setSpeed(speed() + 1);

    if (rpm() >= 5000)
        setRpm(0);
    else
        setRpm(rpm() + 100);
}
