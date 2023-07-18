// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFAMFMTUNER_H
#define QIFAMFMTUNER_H

#include <QtInterfaceFramework/QIfAbstractFeature>
#include <QtIfMedia/qtifmediaglobal.h>
#include <QtCore/QVariant>

QT_BEGIN_NAMESPACE

class QIfAmFmTunerStation;
class QIfAmFmTunerPrivate;

class Q_QTIFMEDIA_EXPORT QIfAmFmTuner : public QIfAbstractFeature
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AmFmTuner)

    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged FINAL)
    Q_PROPERTY(int minimumFrequency READ minimumFrequency NOTIFY minimumFrequencyChanged FINAL)
    Q_PROPERTY(int maximumFrequency READ maximumFrequency NOTIFY maximumFrequencyChanged FINAL)
    Q_PROPERTY(int stepSize READ stepSize NOTIFY stepSizeChanged FINAL)
    Q_PROPERTY(QIfAmFmTuner::Band band READ band WRITE setBand NOTIFY bandChanged FINAL)
    Q_PROPERTY(QIfAmFmTunerStation station READ station NOTIFY stationChanged FINAL)
    Q_PROPERTY(bool scanRunning READ isScanRunning NOTIFY scanRunningChanged FINAL)

public:
    explicit QIfAmFmTuner(QObject *parent = nullptr);

    enum Band {
        AMBand,
        FMBand
    };
    Q_ENUM(Band)

    int frequency() const;
    int minimumFrequency() const;
    int maximumFrequency() const;
    int stepSize() const;
    Band band() const;
    QIfAmFmTunerStation station() const;
    bool isScanRunning() const;

    Q_INVOKABLE void tune(const QIfAmFmTunerStation &station);

public Q_SLOTS:
    void setFrequency(int frequency);
    void setBand(QIfAmFmTuner::Band band);
    void stepUp();
    void stepDown();
    void seekUp();
    void seekDown();
    void startScan();
    void stopScan();

Q_SIGNALS:
    void frequencyChanged(int frequency);
    void minimumFrequencyChanged(int minimumFrequency);
    void maximumFrequencyChanged(int maximumFrequency);
    void stepSizeChanged(int stepSize);
    void bandChanged(QIfAmFmTuner::Band band);
    void stationChanged(const QIfAmFmTunerStation &station);
    void scanRunningChanged(bool scanRunning);
    void scanStarted();
    void scanStopped();

protected:
    QIfAmFmTuner(QIfAmFmTunerPrivate &dd, QObject *parent = nullptr);

    virtual void connectToServiceObject(QIfServiceObject *serviceObject) override;
    virtual void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIfAmFmTuner)
    Q_PRIVATE_SLOT(d_func(), void onFrequencyChanged(int frequency))
    Q_PRIVATE_SLOT(d_func(), void onBandChanged(QIfAmFmTuner::Band band))
    Q_PRIVATE_SLOT(d_func(), void onStationChanged(const QIfAmFmTunerStation &station))
    Q_PRIVATE_SLOT(d_func(), void onMinimumFrequencyChanged(int frequency))
    Q_PRIVATE_SLOT(d_func(), void onMaximumFrequencyChanged(int frequency))
    Q_PRIVATE_SLOT(d_func(), void onStepSizeChanged(int stepSize))
    Q_PRIVATE_SLOT(d_func(), void onScanStatusChanged(bool scanRunning))
};

QT_END_NAMESPACE

#endif // QIFAMFMTUNER_H
