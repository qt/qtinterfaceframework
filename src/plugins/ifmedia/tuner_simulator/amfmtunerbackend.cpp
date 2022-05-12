/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

#include "amfmtunerbackend.h"

#include <QtDebug>

AmFmTunerBackend::AmFmTunerBackend(QObject *parent)
    : QIfAmFmTunerBackendInterface(parent)
    , m_band(QIfAmFmTuner::FMBand)
    , m_timerId(-1)
{
    qRegisterMetaType<QIfAmFmTunerStation>();

    QVector<QIfAmFmTunerStation> fm_stations;
    QIfAmFmTunerStation radioQt;
    radioQt.setId(QStringLiteral("0"));
    radioQt.setStationName(QStringLiteral("Radio Qt"));
    radioQt.setFrequency(87500000);
    radioQt.setBand(QIfAmFmTuner::FMBand);
    fm_stations.append(radioQt);

    QIfAmFmTunerStation qtRocksNonStop;
    qtRocksNonStop.setId(QStringLiteral("1"));
    qtRocksNonStop.setStationName(QStringLiteral("Qt Rocks non-stop"));
    qtRocksNonStop.setFrequency(102500000);
    qtRocksNonStop.setBand(QIfAmFmTuner::FMBand);
    fm_stations.append(qtRocksNonStop);
    BandData fmdata;
    fmdata.m_frequency = 87500000;
    fmdata.m_minimumFrequency = 87500000;
    fmdata.m_maximumFrequency = 108000000;
    fmdata.m_stepSize = 100000;
    fmdata.m_stations = fm_stations;
    m_bandHash.insert(QIfAmFmTuner::FMBand, fmdata);

    BandData amdata;
    amdata.m_frequency = 535000;
    amdata.m_minimumFrequency = 535000;
    amdata.m_maximumFrequency = 1700000;
    amdata.m_stepSize = 10000;
    m_bandHash.insert(QIfAmFmTuner::AMBand, amdata);
}

void AmFmTunerBackend::initialize()
{
    emit bandChanged(m_band);
    emit minimumFrequencyChanged(m_bandHash[m_band].m_minimumFrequency);
    emit maximumFrequencyChanged(m_bandHash[m_band].m_maximumFrequency);
    emit stepSizeChanged(m_bandHash[m_band].m_stepSize);
    emit frequencyChanged(m_bandHash[m_band].m_frequency);
    emit stationChanged(m_bandHash[m_band].m_stations.at(0));
    emit initializationDone();
}

void AmFmTunerBackend::setFrequency(int frequency)
{
    if (m_bandHash[m_band].m_frequency == frequency)
        return;

    if (frequency < m_bandHash[m_band].m_minimumFrequency || frequency > m_bandHash[m_band].m_maximumFrequency) {
        qWarning() << "SIMULATION Frequency out of range";
        return;
    }

    qWarning() << "SIMULATION Frequency changed to" << frequency;

    m_bandHash[m_band].m_frequency = frequency;
    emit frequencyChanged(frequency);
    emit stationChanged(stationAt(m_bandHash[m_band].m_frequency));
}

void AmFmTunerBackend::setBand(QIfAmFmTuner::Band band)
{
    if (m_band == band)
        return;

    qWarning() << "SIMULATION Band changed to" << band;

    m_band = band;
    emit bandChanged(band);
    emit minimumFrequencyChanged(m_bandHash[m_band].m_minimumFrequency);
    emit maximumFrequencyChanged(m_bandHash[m_band].m_maximumFrequency);
    emit stepSizeChanged(m_bandHash[m_band].m_stepSize);
    emit frequencyChanged(m_bandHash[m_band].m_frequency);
    emit stationChanged(stationAt(m_bandHash[m_band].m_frequency));
}

void AmFmTunerBackend::stepUp()
{
    qWarning() << "SIMULATION Step Up";

    int newFreq = m_bandHash[m_band].m_frequency + m_bandHash[m_band].m_stepSize;
    if (newFreq > m_bandHash[m_band].m_maximumFrequency)
        newFreq = m_bandHash[m_band].m_minimumFrequency;

    setFrequency(newFreq);
}

void AmFmTunerBackend::stepDown()
{
    qWarning() << "SIMULATION Step Down";

    int newFreq = m_bandHash[m_band].m_frequency + m_bandHash[m_band].m_stepSize;
    if (newFreq < m_bandHash[m_band].m_minimumFrequency)
        newFreq = m_bandHash[m_band].m_maximumFrequency;

    setFrequency(m_bandHash[m_band].m_frequency - m_bandHash[m_band].m_stepSize);
}

void AmFmTunerBackend::seekUp()
{
    qWarning() << "SIMULATION Seek Up";

    QVector<QIfAmFmTunerStation> stations = m_bandHash[m_band].m_stations;
    if (stations.count() == 0) {
        return;
    } else if (stations.count() == 1) {
        setCurrentStation(stations.at(0));
    } else {
        int index = stationIndexFromFrequency(m_bandHash[m_band].m_frequency);
        index++;
        if (index >= stations.count())
            index = 0;

        setCurrentStation(stations.at(index));
    }
}

void AmFmTunerBackend::seekDown()
{
    qWarning() << "SIMULATION Seek Down";

    QVector<QIfAmFmTunerStation> stations = m_bandHash[m_band].m_stations;
    if (stations.count() == 0) {
        return;
    } else if (stations.count() == 1) {
        setCurrentStation(stations.at(0));
    } else {
        int index = stationIndexFromFrequency(m_bandHash[m_band].m_frequency);
        index--;
        if (index < 0)
            index = stations.count() - 1;

        setCurrentStation(stations.at(index));
    }
}

void AmFmTunerBackend::startScan()
{
    if (m_timerId != -1) {
        qWarning() << "SIMULATION a Scan is already in progress";
        return;
    }

    qWarning() << "SIMULATION Scan started";

    emit scanStatusChanged(true);
    seekUp();
    m_timerId = startTimer(5000);
}

void AmFmTunerBackend::stopScan()
{
    if (m_timerId == -1) {
        qWarning() << "SIMULATION no Scan is currently in progress, which can be stopped";
        return;
    }

    qWarning() << "SIMULATION Scan stopped";

    killTimer(m_timerId);
    m_timerId = -1;
    emit scanStatusChanged(false);
}

void AmFmTunerBackend::setCurrentStation(const QIfAmFmTunerStation &station)
{
    m_bandHash[m_band].m_frequency = station.frequency();

    qWarning() << "SIMULATION Station changed to" << station.stationName() << station.frequency();

    emit frequencyChanged(station.frequency());
    emit stationChanged(station);
}

int AmFmTunerBackend::stationIndexFromFrequency(int frequency) const
{
    QVector<QIfAmFmTunerStation> stations = m_bandHash[m_band].m_stations;
    for (int i=0; i < stations.count(); i++) {
        const QIfAmFmTunerStation& station = stations.at(i);
        if (station.frequency() == frequency)
            return i;
    }

    return -1;
}

QIfAmFmTunerStation AmFmTunerBackend::stationAt(int frequency) const
{
    int index = stationIndexFromFrequency(frequency);
    if (index != -1)
        return m_bandHash[m_band].m_stations.at(index);

    return QIfAmFmTunerStation();
}

void AmFmTunerBackend::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    seekUp();
}
