/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
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

#ifndef QTIVICLIMATECONTROLBACKENDINTERFACE_H
#define QTIVICLIMATECONTROLBACKENDINTERFACE_H

#include <QtCore/QObject>

#include <QtIVICore/qtivizonedfeatureinterface.h>
#include <QtIVICore/qtivipropertyattribute.h>
#include <QtIVIVehicleFunctions/qtiviclimatecontrol.h>

QT_BEGIN_NAMESPACE

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIClimateControlBackendInterface : public QtIVIZonedFeatureInterface
{
    Q_OBJECT

public:
    QtIVIClimateControlBackendInterface(QObject *parent = 0);

    virtual void setTargetTemperature(int value, const QString &zone) = 0;
    virtual void setSeatCooler(int value, const QString &zone) = 0;
    virtual void setSeatHeater(int value, const QString &zone) = 0;
    virtual void setSteeringWheelHeater(int value, const QString &zone) = 0;
    virtual void setFanSpeedLevel(int value, const QString &zone) = 0;

    virtual void setAirflowDirection(QtIVIClimateControl::AirflowDirection airflowDirection, const QString &zone) = 0;
    virtual void setAirConditioningEnabled(bool enabled, const QString &zone) = 0;
    virtual void setHeaterEnabled(bool enabled, const QString &zone) = 0;
    virtual void setAirRecirculationEnabled(bool enabled, const QString &zone) = 0;

Q_SIGNALS:
    void targetTemperatureChanged(int value, const QString &zone = QString());
    void targetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
    void seatCoolerChanged(int value, const QString &zone = QString());
    void seatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
    void seatHeaterChanged(int value, const QString &zone = QString());
    void seatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
    void steeringWheelHeaterChanged(int value, const QString &zone = QString());
    void steeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
    void fanSpeedLevelChanged(int value, const QString &zone = QString());
    void fanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
    void airflowDirectionChanged(QtIVIClimateControl::AirflowDirection value, const QString &zone = QString());
    void airflowDirectionAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirection> &attribute, const QString &zone = QString());
    void airConditioningEnabledChanged(bool value, const QString &zone = QString());
    void airConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone = QString());
    void heaterEnabledChanged(bool value, const QString &zone = QString());
    void heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone = QString());
    void airRecirculationEnabledChanged(bool value, const QString &zone = QString());
    void airRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone = QString());
};

QT_END_NAMESPACE

#endif // QTIVICLIMATECONTROLBACKENDINTERFACE_H

