// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef INSTRUMENTCLUSTER_H
#define INSTRUMENTCLUSTER_H

#include <QObject>
#include "warning.h"
#include "instrumentclustermodule.h"

class InstrumentCluster : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int rpm READ rpm WRITE setRpm NOTIFY rpmChanged)
    Q_PROPERTY(qreal fuel READ fuel WRITE setFuel NOTIFY fuelChanged)
    Q_PROPERTY(qreal temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)
    Q_PROPERTY(InstrumentClusterModule::SystemType systemType READ systemType WRITE setSystemType NOTIFY systemTypeChanged)
    Q_PROPERTY(Warning currentWarning READ currentWarning WRITE setCurrentWarning NOTIFY currentWarningChanged)

public:
    explicit InstrumentCluster(QObject *parent = nullptr);

    Q_INVOKABLE int speed() const;
    Q_INVOKABLE int rpm() const;
    Q_INVOKABLE qreal fuel() const;
    Q_INVOKABLE qreal temperature() const;
    Q_INVOKABLE InstrumentClusterModule::SystemType systemType() const;
    Q_INVOKABLE Warning currentWarning() const;

signals:
    void speedChanged(int speed);
    void rpmChanged(int rpm);
    void fuelChanged(qreal fuel);
    void temperatureChanged(qreal temperature);
    void systemTypeChanged(InstrumentClusterModule::SystemType systemType);
    void currentWarningChanged(Warning currentWarning);

public slots:
    void setSpeed(int speed);
    void setRpm(int rpm);
    void setFuel(qreal fuel);
    void setTemperature(qreal temperature);
    void setSystemType(InstrumentClusterModule::SystemType systemType);
    void setCurrentWarning(const Warning &currentWarning);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    int m_speed;
    int m_rpm;
    qreal m_fuel;
    qreal m_temperature;
    InstrumentClusterModule::SystemType m_systemType;
    Warning m_currentWarning;
};

#endif // INSTRUMENTCLUSTER_H
