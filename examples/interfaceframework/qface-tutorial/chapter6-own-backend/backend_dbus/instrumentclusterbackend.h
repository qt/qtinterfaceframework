// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef INSTRUMENTCLUSTERBACKEND_H
#define INSTRUMENTCLUSTERBACKEND_H

#include "instrumentclusterbackendinterface.h"
#include "instrumentcluster_interface.h"

class InstrumentClusterBackend : public InstrumentClusterBackendInterface
{
    Q_OBJECT
public:
    InstrumentClusterBackend(QObject *parent = nullptr);

public:
    void initialize() override;

    void setupConnection();
    void fetchSpeed();
    void fetchRpm();
    void fetchFuel();
    void fetchTemperature();
    void fetchSystemType();
    void fetchCurrentWarning();
    void checkInitDone();

public Q_SLOTS:
    void onSpeedChanged(int speed);
    void onRpmChanged(int rpm);
    void onFuelChanged(qreal fuel);
    void onTemperaturChanged(qreal temperature);
    void onSystemTypeChanged(InstrumentClusterModule::SystemType systemType);
    void onCurrentWarningChanged(const Warning &warning);

private:
    ExampleIfInstrumentClusterInterface *m_client;
    QStringList m_fetchList;
    int m_speed;
    int m_rpm;
    qreal m_fuel;
    qreal m_temperature;
    InstrumentClusterModule::SystemType m_systemType;
    Warning m_warning;
};

#endif // INSTRUMENTCLUSTERBACKEND_H
