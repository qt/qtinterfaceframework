/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
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

#include "qtiviclimatecontrolbackendinterface.h"

/*!
 * \class QtIVIClimateControlBackendInterface
 * \inmodule QtIVIVehicleFunctions
 * \ingroup backends
 * \inherits QtIVIZonedFeatureInterface
 * \brief The QtIVIClimateControlBackendInterface defines the interface for backends to the
 * QtIVIClimateControl feature class.
 *
 * The QtIVIClimateControlBackendInterface is the interface used by \l QtIVIClimateControl
 *
 * The interface is discovered by a \l QtIVIClimateControl object, which connects to it and sets up
 * the connections to it.
 *
 * \sa QtIVIClimateControl
 */

/*!
 * \fn QtIVIClimateControlBackendInterface::QtIVIClimateControlBackendInterface(QObject *parent=0)
 *
 * Constructs a backend interface.
 *
 * The \a parent is sent to the QObject constructor.
 */
QtIVIClimateControlBackendInterface::QtIVIClimateControlBackendInterface(QObject *parent)
    : QtIVIZonedFeatureInterface(parent)
{
}

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setTargetTemperature(int value, const QString &zone) = 0
 *
 * Sets the target temperature of \a zone to \a value, where the \a value is expressed in
 * centigrades and may be range limited by the backend.
 *
 * This method is expected to emit a \l targetTemperatureChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa targetTemperatureChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSeatCooler(int value, const QString &zone) = 0
 *
 * Sets the seat ventilation level of \a zone to \a value, where \a value can be \c 0 (off) or
 * between \c 1 (least cool) to \c 10 (coolest).
 *
 * This method is expected to emit a \l seatCoolerChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatCoolerChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSeatHeater(int value, const QString &zone) = 0;
 *
 * Sets the seat heater level of \a zone to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l seatHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa seatHeaterChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirflowDirections(QtIVIClimateControl::AirflowDirections, const QString &zone) = 0;
 *
 * Sets the \a zone air flow directions to \a airflowDirections.
 *
 * This method is expected to emit the \l airflowDirectionsChanged() signal when receiving a
 * new \a airflowDirections.
 *
 * \sa airflowDirectionsChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirConditioningEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone air conditioning based on \a enabled.
 *
 * This method is expected to emit the \l airConditioningEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airConditioningEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setHeaterEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone heater based on \a enabled.
 *
 * This method is expected to emit the \l heaterEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa heaterEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setAirRecirculationEnabled(bool enabled, const QString &zone) = 0
 *
 * Enables or disables the \a zone air recirculation based on \a enabled.
 *
 * This method is expected to emit the \l airRecirculationEnabledChanged() signal when receiving a
 * new \a enabled.
 *
 * \sa airRecirculationEnabledChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setSteeringWheelHeater(int value, const QString &zone) = 0
 *
 * Sets the steering wheel heater level of \a zone to \a value, where \a value can be \c 0 (off)
 * or between \c 1 (least warm) to \c 10 (warmest).
 *
 * This method is expected to emit a \l steeringWheelHeaterChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa steeringWheelHeaterChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::setFanSpeedLevel(int value, const QString &zone) = 0
 *
 * Sets the fan speed level of \a zone to \a value, where \a value can be \c 0 (off) or between
 * \c 1 (weakest) to \c 10 (strongest).
 *
 * This method is expected to emit a \l fanSpeedLevelChanged() signal when receiving a new
 * \a value. The signal is expected to be emitted if the given \a value is out of range and no
 * actual change takes place.
 *
 * \sa fanSpeedLevelChanged()
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::targetTemperatureChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the target temperature for \a zone is changed to \a value, where
 * value is expressed in centigrades.
 *
 * \sa setTargetTemperature()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::targetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the target temperature attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::seatCoolerChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the seat cooler level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least cool) to \c 10 (coolest).
 *
 * \sa setSeatCooler()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::seatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the seat cooler level attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::seatHeaterChanged(int value, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the seat heater level is changed for \a zone to \a value, where the
 * \a value can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSeatHeater()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::seatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the seat heater attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airflowDirectionsChanged(QtIVIClimateControl::AirflowDirections airflowDirections, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone airflow directions changed to \a airflowDirections.
 *
 * \sa setAirflowDirections()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the zone airflow direction attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airConditioningEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone air conditioning state is changed to \a enabled.
 *
 * \sa setAirConditioningEnabled()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the air conditioning state attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::heaterEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone heater state is changed to \a enabled.
 *
 * \sa setHeaterEnabled()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the heater state attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged(bool enabled, const QString &zone = QString()) = 0
 *
 * The signal is emitted when the \a zone air recirculation state is changed to \a enabled.
 *
 * \sa setAirRecirculationEnabled()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::airRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the air recirculation state attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals ie emitted when the steering wheel heater level of \a zone is changed to \a level, where the
 * \a level can be \c 0 (off) or between \c 1 (least warm) to \c 10 (warmest).
 *
 * \sa setSteeringWheelHeater()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::steeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the steering wheel heater level attribute for \a zone is changed to \a attribute.
 */

/*!
 * \fn virtual void QtIVIClimateControlBackendInterface::fanSpeedLevelChanged(int level, const QString &zone = QString()) = 0
 *
 * The signals ie emitted when the fan speel level of \a zone is changed to \a level, where the \a level
 * can be \c 0 (off) or between \c 1 (weakest) to \c 10 (strongest).
 *
 * \sa setFanSpeedLevel()
 */

/*!
 * \fn void QtIVIClimateControlBackendInterface::fanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute, const QString &zone = QString());
 *
 * The signal is emitted when the fan speed level attribute for \a zone is changed to \a attribute.
 */
