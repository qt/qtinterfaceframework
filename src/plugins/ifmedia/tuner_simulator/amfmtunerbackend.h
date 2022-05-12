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

#ifndef AMFMTUNERBACKEND_H
#define AMFMTUNERBACKEND_H

#include <QtCore/QVector>
#include <QtIfMedia/QIfAmFmTunerBackendInterface>
#include <QtIfMedia/QIfTunerStation>

class AmFmTunerBackend : public QIfAmFmTunerBackendInterface
{
    Q_OBJECT
public:
    explicit AmFmTunerBackend(QObject *parent = nullptr);

    void initialize() override;
    void setFrequency(int frequency) override;
    void setBand(QIfAmFmTuner::Band band) override;
    void stepUp() override;
    void stepDown() override;
    void seekUp() override;
    void seekDown() override;
    void startScan() override;
    void stopScan() override;

private:
    void setCurrentStation(const QIfAmFmTunerStation &station);
    int stationIndexFromFrequency(int frequency) const;
    QIfAmFmTunerStation stationAt(int frequency) const;
    void timerEvent(QTimerEvent *event) override;

    QIfAmFmTuner::Band m_band;
    struct BandData {
        QVector<QIfAmFmTunerStation> m_stations;
        int m_stepSize;
        int m_frequency;
        int m_minimumFrequency;
        int m_maximumFrequency;
    };
    QHash<QIfAmFmTuner::Band, BandData> m_bandHash;
    int m_timerId;

    friend class SearchAndBrowseBackend;
};

#endif // AMFMTUNERBACKEND_H
