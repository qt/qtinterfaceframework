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

#ifndef QIFAMFMTUNER_P_H
#define QIFAMFMTUNER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qtifmediaglobal_p.h"
#include <QtInterfaceFramework/private/qifabstractfeature_p.h>

#include "qifamfmtuner.h"
#include "qifamfmtunerbackendinterface.h"

#include <QtIfMedia/QIfTunerStation>

QT_BEGIN_NAMESPACE

class QIfAmFmTunerPrivate : public QIfAbstractFeaturePrivate
{
public:
    QIfAmFmTunerPrivate(const QString &interface, QIfAmFmTuner *parent);

    void clearToDefaults();
    void onFrequencyChanged(int frequency);
    void onMinimumFrequencyChanged(int frequency);
    void onMaximumFrequencyChanged(int frequency);
    void onStepSizeChanged(int stepSize);
    void onBandChanged(QIfAmFmTuner::Band band);
    void onStationChanged(const QIfAmFmTunerStation &station);
    void onScanStatusChanged(bool scanRunning);

    QIfAmFmTunerBackendInterface *tunerBackend() const;

    QIfAmFmTuner * const q_ptr;
    int m_frequency;
    int m_minimumFrequency;
    int m_maximumFrequency;
    int m_stepSize;
    QIfAmFmTuner::Band m_band;
    QIfAmFmTunerStation m_station;
    bool m_scanRunning;

    Q_DECLARE_PUBLIC(QIfAmFmTuner)
};

QT_END_NAMESPACE

#endif // QIFAMFMTUNER_P_H
