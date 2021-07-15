/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QIFAMFMTUNER_H
#define QIFAMFMTUNER_H

#include <QtInterfaceFramework/QIfAbstractFeature>
#include <QtIfMedia/qtifmediaglobal.h>
#include <QVariant>

QT_BEGIN_NAMESPACE

class QIfAmFmTunerStation;
class QIfAmFmTunerPrivate;

class Q_QTIFMEDIA_EXPORT QIfAmFmTuner : public QIfAbstractFeature
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AmFmTuner)

    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(int minimumFrequency READ minimumFrequency NOTIFY minimumFrequencyChanged)
    Q_PROPERTY(int maximumFrequency READ maximumFrequency NOTIFY maximumFrequencyChanged)
    Q_PROPERTY(int stepSize READ stepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(QIfAmFmTuner::Band band READ band WRITE setBand NOTIFY bandChanged)
    Q_PROPERTY(QIfAmFmTunerStation station READ station NOTIFY stationChanged)
    Q_PROPERTY(bool scanRunning READ isScanRunning NOTIFY scanRunningChanged)

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
