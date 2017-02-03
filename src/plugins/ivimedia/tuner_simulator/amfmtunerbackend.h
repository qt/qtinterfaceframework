/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#ifndef AMFMTUNERBACKEND_H
#define AMFMTUNERBACKEND_H

#include <QtIviMedia/QIviAmFmTunerBackendInterface>
#include <QtIviMedia/QIviTunerStation>
#include <QtCore/QVector>

class AmFmTunerBackend : public QIviAmFmTunerBackendInterface
{
    Q_OBJECT
public:
    explicit AmFmTunerBackend(QObject *parent = 0);

    virtual void initialize() Q_DECL_OVERRIDE;
    virtual void setFrequency(int frequency) Q_DECL_OVERRIDE;
    virtual void setBand(QIviAmFmTuner::Band band) Q_DECL_OVERRIDE;
    virtual void stepUp() Q_DECL_OVERRIDE;
    virtual void stepDown() Q_DECL_OVERRIDE;
    virtual void seekUp() Q_DECL_OVERRIDE;
    virtual void seekDown() Q_DECL_OVERRIDE;
    virtual void startScan() Q_DECL_OVERRIDE;
    virtual void stopScan() Q_DECL_OVERRIDE;

private:
    void setCurrentStation(const QIviAmFmTunerStation &station);
    int stationIndexFromFrequency(int frequency) const;
    QIviAmFmTunerStation stationAt(int frequency) const;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

    QIviAmFmTuner::Band m_band;
    struct BandData {
        QVector<QIviAmFmTunerStation> m_stations;
        int m_stepSize;
        int m_frequency;
        int m_minimumFrequency;
        int m_maximumFrequency;
    };
    QHash<QIviAmFmTuner::Band, BandData> m_bandHash;
    int m_timerId;

    friend class SearchAndBrowseBackend;
};

#endif // AMFMTUNERBACKEND_H
