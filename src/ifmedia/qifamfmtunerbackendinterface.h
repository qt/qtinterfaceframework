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
