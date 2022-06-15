// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFAMFMTUNERBACKENDINTERFACE_H
#define QIFAMFMTUNERBACKENDINTERFACE_H

#include <QtInterfaceFramework/QIfFeatureInterface>
#include <QtIfMedia/qtifmediaglobal.h>
#include <QtIfMedia/QIfAmFmTuner>

QT_BEGIN_NAMESPACE

class QIfPlayableItem;

class Q_QTIFMEDIA_EXPORT QIfAmFmTunerBackendInterface : public QIfFeatureInterface
{
    Q_OBJECT

public:
    explicit QIfAmFmTunerBackendInterface(QObject *parent = nullptr);

    virtual void setFrequency(int frequency) = 0;
    virtual void setBand(QIfAmFmTuner::Band band) = 0;
    virtual void stepUp() = 0;
    virtual void stepDown() = 0;
    virtual void seekUp() = 0;
    virtual void seekDown() = 0;
    virtual void startScan() = 0;
    virtual void stopScan() = 0;

Q_SIGNALS:
    void frequencyChanged(int frequency);
    void minimumFrequencyChanged(int minimumFrequency);
    void maximumFrequencyChanged(int maximumFrequency);
    void stepSizeChanged(int stepSize);
    void bandChanged(QIfAmFmTuner::Band band);
    void stationChanged(const QIfAmFmTunerStation &station);
    void scanStatusChanged(bool scanRunning);
};

#define QIfAmFmTuner_iid "org.qt-project.interfaceframework.AmFmTuner/1.0"

QT_END_NAMESPACE

#endif // QIFAMFMTUNERBACKENDINTERFACE_H
