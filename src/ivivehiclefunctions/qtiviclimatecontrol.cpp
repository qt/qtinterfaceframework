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

#include "qtiviclimatecontrol.h"
#include "qtiviclimatecontrol_p.h"
#include "qtiviclimatecontrolbackendinterface.h"
#include <QtIVICore/QtIVIPropertyFactory>

QtIVIClimateControlPrivate::QtIVIClimateControlPrivate(const QString &interface, const QString &zone, QtIVIClimateControl *parent)
    : QtIVIAbstractZonedFeaturePrivate(interface, zone, parent)
    , m_airflowDirections(0)
    , m_airConditioning(false)
    , m_heater(false)
    , m_airRecirculation(false)
    , m_targetTemperature(0)
    , m_seatCooler(0)
    , m_seatHeater(0)
    , m_steeringWheelHeater(0)
    , m_fanSpeedLevel(0)
    , q_ptr(parent)
{
}

void QtIVIClimateControlPrivate::init()
{
    Q_Q(QtIVIClimateControl);
    m_airFlowDirectionProperty = QtIVIPropertyFactory<QtIVIClimateControl::AirflowDirections>::create(q,
                                                                                                     &QtIVIClimateControl::airflowDirectionsAttribute,
                                                                                                     &QtIVIClimateControl::airflowDirectionsAttributeChanged,
                                                                                                     &QtIVIClimateControl::airflowDirections,
                                                                                                     &QtIVIClimateControl::airflowDirectionsChanged,
                                                                                                     &QtIVIClimateControl::setAirflowDirections);
    m_airConditioningProperty = QtIVIPropertyFactory<bool>::create(q,
                                                                   &QtIVIClimateControl::airConditioningAttribute,
                                                                   &QtIVIClimateControl::airConditioningAttributeChanged,
                                                                   &QtIVIClimateControl::isAirConditioningEnabled,
                                                                   &QtIVIClimateControl::airConditioningEnabledChanged,
                                                                   &QtIVIClimateControl::setAirConditioningEnabled);
    m_heaterProperty = QtIVIPropertyFactory<bool>::create(q,
                                                          &QtIVIClimateControl::heaterAttribute,
                                                          &QtIVIClimateControl::heaterAttributeChanged,
                                                          &QtIVIClimateControl::isHeaterEnabled,
                                                          &QtIVIClimateControl::heaterEnabledChanged,
                                                          &QtIVIClimateControl::setHeaterEnabled);
    m_airRecirculationProperty = QtIVIPropertyFactory<bool>::create(q,
                                                                    &QtIVIClimateControl::airRecirculationAttribute,
                                                                    &QtIVIClimateControl::airRecirculationAttributeChanged,
                                                                    &QtIVIClimateControl::isAirRecirculationEnabled,
                                                                    &QtIVIClimateControl::airRecirculationEnabledChanged,
                                                                    &QtIVIClimateControl::setAirRecirculationEnabled);
    m_targetTemperatureProperty = QtIVIPropertyFactory<int>::create(q,
                                                                    &QtIVIClimateControl::targetTemperatureAttribute,
                                                                    &QtIVIClimateControl::targetTemperatureAttributeChanged,
                                                                    &QtIVIClimateControl::targetTemperature,
                                                                    &QtIVIClimateControl::targetTemperatureChanged,
                                                                    &QtIVIClimateControl::setTargetTemperature);
    m_seatCoolerProperty = QtIVIPropertyFactory<int>::create(q,
                                                             &QtIVIClimateControl::seatCoolerAttribute,
                                                             &QtIVIClimateControl::seatCoolerAttributeChanged,
                                                             &QtIVIClimateControl::seatCooler,
                                                             &QtIVIClimateControl::seatCoolerChanged,
                                                             &QtIVIClimateControl::setSeatCooler);
    m_seatHeaterProperty = QtIVIPropertyFactory<int>::create(q,
                                                             &QtIVIClimateControl::seatHeaterAttribute,
                                                             &QtIVIClimateControl::seatHeaterAttributeChanged,
                                                             &QtIVIClimateControl::seatHeater,
                                                             &QtIVIClimateControl::seatHeaterChanged,
                                                             &QtIVIClimateControl::setSeatHeater);
    m_steeringWheelHeaterProperty = QtIVIPropertyFactory<int>::create(q,
                                                                      &QtIVIClimateControl::steeringWheelHeaterAttribute,
                                                                      &QtIVIClimateControl::steeringWheelHeaterAttributeChanged,
                                                                      &QtIVIClimateControl::steeringWheelHeater,
                                                                      &QtIVIClimateControl::steeringWheelHeaterChanged,
                                                                      &QtIVIClimateControl::setSteeringWheelHeater);
    m_fanSpeedLevelProperty = QtIVIPropertyFactory<int>::create(q,
                                                                &QtIVIClimateControl::fanSpeedLevelAttribute,
                                                                &QtIVIClimateControl::fanSpeedLevelAttributeChanged,
                                                                &QtIVIClimateControl::fanSpeedLevel,
                                                                &QtIVIClimateControl::fanSpeedLevelChanged,
                                                                &QtIVIClimateControl::setFanSpeedLevel);
}

void QtIVIClimateControlPrivate::clearToDefaults()
{
    m_airConditioning = false;
    m_heater = false;
    m_airRecirculation = false;
    m_targetTemperature = 0;
    m_seatCooler = 0;
    m_seatHeater = 0;
    m_steeringWheelHeater = 0;
    m_fanSpeedLevel = 0;
}

void QtIVIClimateControlPrivate::onAirflowDirectionsChanged(QtIVIClimateControl::AirflowDirections value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airflowDirections = value;
    emit f->airflowDirectionsChanged(value);
}

void QtIVIClimateControlPrivate::onAirflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &airflowDirectionsAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airflowDirectionsAttribute = airflowDirectionsAttribute;
    emit f->airflowDirectionsAttributeChanged(airflowDirectionsAttribute);
}

void QtIVIClimateControlPrivate::onAirConditioningEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airConditioning = enabled;
    emit f->airConditioningEnabledChanged(enabled);
}

void QtIVIClimateControlPrivate::onAirConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &airConditioningEnabledAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airConditioningAttribute = airConditioningEnabledAttribute;
    emit f->airConditioningAttributeChanged(airConditioningEnabledAttribute);
}

void QtIVIClimateControlPrivate::onHeaterEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heater = enabled;
    emit f->heaterEnabledChanged(enabled);
}

void QtIVIClimateControlPrivate::onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &heaterEnabledAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_heaterAttribute = heaterEnabledAttribute;
    emit f->heaterAttributeChanged(heaterEnabledAttribute);
}

void QtIVIClimateControlPrivate::onAirRecirculationEnabledChanged(bool enabled, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airRecirculation = enabled;
    emit f->airRecirculationEnabledChanged(enabled);
}

void QtIVIClimateControlPrivate::onAirRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &airRecirculationEnabledAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_airRecirculationAttribute = airRecirculationEnabledAttribute;
    emit f->airRecirculationAttributeChanged(airRecirculationEnabledAttribute);
}

void QtIVIClimateControlPrivate::onSteeringWheelHeaterChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_steeringWheelHeater = value;
    emit f->steeringWheelHeaterChanged(value);
}

void QtIVIClimateControlPrivate::onSteeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &steeringWheelHeaterAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_steeringWheelHeaterAttribute = steeringWheelHeaterAttribute;
    emit f->steeringWheelHeaterAttributeChanged(steeringWheelHeaterAttribute);
}

void QtIVIClimateControlPrivate::onFanSpeedLevelChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_fanSpeedLevel = value;
    emit f->fanSpeedLevelChanged(value);
}

void QtIVIClimateControlPrivate::onFanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &fanSpeedAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_fanSpeedLevelAttribute = fanSpeedAttribute;
    emit f->fanSpeedLevelAttributeChanged(fanSpeedAttribute);
}

void QtIVIClimateControlPrivate::onTargetTemperatureChanged(int temperature, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_targetTemperature = temperature;
    emit f->targetTemperatureChanged(temperature);
}

void QtIVIClimateControlPrivate::onTargetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &temperatureAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_targetTemperatureAttribute = temperatureAttribute;
    emit f->targetTemperatureAttributeChanged(temperatureAttribute);
}

void QtIVIClimateControlPrivate::onSeatCoolerChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatCooler = value;
    emit f->seatCoolerChanged(value);
}

void QtIVIClimateControlPrivate::onSeatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &seatCoolerAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatCoolerAttribute = seatCoolerAttribute;
    emit f->seatCoolerAttributeChanged(seatCoolerAttribute);
}

void QtIVIClimateControlPrivate::onSeatHeaterChanged(int value, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatHeater = value;
    emit f->seatHeaterChanged(value);
}

void QtIVIClimateControlPrivate::onSeatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &seatHeaterAttribute, const QString &zone)
{
    Q_Q(QtIVIClimateControl);
    QtIVIClimateControl *f = qobject_cast<QtIVIClimateControl*>(q->zoneAt(zone));
    if (!f)
        f = q;
    if (f->zone() != zone)
        return;
    f->d_func()->m_seatHeaterAttribute = seatHeaterAttribute;
    emit f->seatHeaterAttributeChanged(seatHeaterAttribute);
}

QtIVIClimateControlBackendInterface *QtIVIClimateControlPrivate::climateControlBackend()
{
    Q_Q(QtIVIClimateControl);
    return qobject_cast<QtIVIClimateControlBackendInterface*>(q->backend());
}

/*!
   \class QtIVIClimateControl
   \inmodule QtIVIVehicleFunctions
   \brief Provides an interface to the climate control.

   The QtIVIClimateControl provides an interface to the climate control of the vehicle.

   The climate control properties are divided into two categories: central or zoned. The central
   properties are exposed directly through the QtIVIClimateControl and the zoned properties are
   exposed through zone objects. The zones are retrieved using the \l zoneAt method.

   The QtIVIClimateControl expects a single backend to be available. It is recommended to use it
   with \l {QtIVIAbstractFeature::}{discoveryMode} set to \l AutoDiscovery.
*/

/*!
    \qmltype ClimateControl
    \instantiates QtIVIClimateControl
    \inqmlmodule QtIVIVehicleFunctions
    \inherits AbstractZonedFeature
    \brief The ClimateControl provides a QML interface to the climate control of the vehicle.
*/

/*!
   \enum QtIVIClimateControl::AirflowDirections
   \value Windshield
          Direct airflow along the windshield.
   \value Dashboard
          Direct airflow through the dashboard.
   \value Floor
          Direct airflow to the floor.
*/

/*!
   Constructs a climate control object to the given \a zone.

   If \a zone is not provided the General zone will be created.

   The \a parent argument is passed on to the \l QtIVIAbstractZonedFeature base class.
*/
QtIVIClimateControl::QtIVIClimateControl(const QString &zone, QObject* parent)
    : QtIVIAbstractZonedFeature(*new QtIVIClimateControlPrivate(QtIVIStringClimateControlInterfaceName, zone, this), parent)
{
    Q_D(QtIVIClimateControl);
    d->init();
    qRegisterMetaType<QtIVIPropertyAttribute<int>>();
    qRegisterMetaType<QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections>>();
    qRegisterMetaType<QtIVIPropertyAttribute<bool>>();
}

QtIVIClimateControl::~QtIVIClimateControl()
{
}

/*!
   \reimp
*/
void QtIVIClimateControl::connectToServiceObject(QtIVIServiceObject *serviceObject)
{
    Q_D(QtIVIClimateControl);
    QtIVIAbstractZonedFeature::connectToServiceObject(serviceObject);

    QtIVIClimateControlBackendInterface* backend = d->climateControlBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureChanged,
            d, &QtIVIClimateControlPrivate::onTargetTemperatureChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::targetTemperatureAttributeChanged,
            d, &QtIVIClimateControlPrivate::onTargetTemperatureAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerChanged,
            d, &QtIVIClimateControlPrivate::onSeatCoolerChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatCoolerAttributeChanged,
            d, &QtIVIClimateControlPrivate::onSeatCoolerAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterChanged,
            d, &QtIVIClimateControlPrivate::onSeatHeaterChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::seatHeaterAttributeChanged,
            d, &QtIVIClimateControlPrivate::onSeatHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterChanged,
            d, &QtIVIClimateControlPrivate::onSteeringWheelHeaterChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::steeringWheelHeaterAttributeChanged,
            d, &QtIVIClimateControlPrivate::onSteeringWheelHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelChanged,
            d, &QtIVIClimateControlPrivate::onFanSpeedLevelChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::fanSpeedLevelAttributeChanged,
            d, &QtIVIClimateControlPrivate::onFanSpeedLevelAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionsChanged,
            d, &QtIVIClimateControlPrivate::onAirflowDirectionsChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airflowDirectionsAttributeChanged,
            d, &QtIVIClimateControlPrivate::onAirflowDirectionsAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airConditioningEnabledChanged,
            d, &QtIVIClimateControlPrivate::onAirConditioningEnabledChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airConditioningAttributeChanged,
            d, &QtIVIClimateControlPrivate::onAirConditioningAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::heaterEnabledChanged,
            d, &QtIVIClimateControlPrivate::onHeaterEnabledChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::heaterAttributeChanged,
            d, &QtIVIClimateControlPrivate::onHeaterAttributeChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airRecirculationEnabledChanged,
            d, &QtIVIClimateControlPrivate::onAirRecirculationEnabledChanged);
    QObjectPrivate::connect(backend, &QtIVIClimateControlBackendInterface::airRecirculationAttributeChanged,
            d, &QtIVIClimateControlPrivate::onAirRecirculationAttributeChanged);

    backend->initializeAttributes();
}

/*!
   \reimp
*/
void QtIVIClimateControl::clearServiceObject()
{
    Q_D(QtIVIClimateControl);
    d->clearToDefaults();
}

QtIVIClimateControl::QtIVIClimateControl(QtIVIClimateControlPrivate &dd, QObject *parent)
    : QtIVIAbstractZonedFeature(dd, parent)
{
    Q_D(QtIVIClimateControl);
    d->init();
}

/*!
   \reimp
*/
QtIVIAbstractZonedFeature *QtIVIClimateControl::createZoneFeature(const QString &zone)
{
    return new QtIVIClimateControl(zone, this);
}
/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::airConditioning}

   \e value is true if the air conditioning is enabled.
*/
/*!
   \property QtIVIClimateControl::airConditioning

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if air conditioning is enabled.

   \sa AttributeSystem
   \sa isAirConditioningEnabled() setAirConditioningEnabled() airConditioningAttribute()
 */
/*!
 * Returns true if air conditioning is enabled.
 *
 * \sa setAirConditioningEnabled() airConditioningEnabledChanged() airConditioningAttribute()
 */
bool QtIVIClimateControl::isAirConditioningEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airConditioning;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air conditioning property.
 *
 * \sa setAirConditioningEnabled() isAirConditioningEnabled() airConditioningEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::airConditioningAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airConditioningAttribute;
}

QtIVIProperty *QtIVIClimateControl::airConditioningProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airConditioningProperty;
}

/*!
   \qmlqtivipropertyEnum {QtIVIVehicleFunctions::ClimateControl::airflowDirections}

   \e value holds the airflow directions.
   Available values are:
   \value Windshield
          Direct airflow along the windshield.
   \value Dashboard
          Direct airflow through the dashboard.
   \value Floor
          Direct airflow to the floor.
 */
/*!
   \property QtIVIClimateControl::airflowDirections

   Holds a QtIVIProperty of type \e QtIVIClimateControl::AirflowDirections where the QtIVIProperty::value() function returns the current air flow directions.

   \sa AttributeSystem
   \sa airflowDirections() setAirflowDirections() airflowDirectionsAttribute()
 */
/*!
 * Returns the current air flow direction.
 *
 * \sa setAirflowDirections() airflowDirectionsChanged() airflowDirectionsAttribute()
 */
QtIVIClimateControl::AirflowDirections QtIVIClimateControl::airflowDirections() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airflowDirections;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air flow property
 *
 * \sa setAirflowDirections() airflowDirections() airflowDirectionsChanged()
 */
QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> QtIVIClimateControl::airflowDirectionsAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airflowDirectionsAttribute;
}

QtIVIProperty *QtIVIClimateControl::airflowDirectionsProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airFlowDirectionProperty;
}

/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::heater}

   \e value is true if the heater is enabled.
*/
/*!
   \property QtIVIClimateControl::heater

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if the heater is enabled.

   \sa AttributeSystem
   \sa isHeaterEnabled() setHeaterEnabled() heaterAttribute()
 */

/*!
 * Returns true if the heater is enabled.
 *
 * \sa setHeaterEnabled() heaterEnabledChanged() heaterAttribute()
 */
bool QtIVIClimateControl::isHeaterEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_heater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the heater property.
 *
 * \sa setHeaterEnabled() isHeaterEnabled() heaterEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::heaterAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_heaterAttribute;
}

QtIVIProperty *QtIVIClimateControl::heaterProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_heaterProperty;
}

/*!
   \qmlqtivipropertyBool {QtIVIVehicleFunctions::ClimateControl::airRecirculation}

   \e value is true if the air recirculation is enabled.
*/
/*!
   \property QtIVIClimateControl::airRecirculation

   Holds a QtIVIProperty of type \e bool where the QtIVIProperty::value() function indicates if air recirculation is enabled.

   \sa AttributeSystem
   \sa isAirRecirculationEnabled() setAirRecirculationEnabled() airRecirculationAttribute()
 */
/*!
 * Returns true if air recirculation is enabled.
 *
 * \sa setAirRecirculationEnabled() airRecirculationEnabledChanged() airRecirculationAttribute()
 */
bool QtIVIClimateControl::isAirRecirculationEnabled() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airRecirculation;
}

/*!
 * Returns the attribute defining the boundaries and availability of the air recirculation property.
 *
 * \sa setAirRecirculationEnabled() isAirRecirculationEnabled() airRecirculationEnabledChanged()
 */
QtIVIPropertyAttribute<bool> QtIVIClimateControl::airRecirculationAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airRecirculationAttribute;
}

QtIVIProperty *QtIVIClimateControl::airRecirculationProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_airRecirculationProperty;
}

/*!
   \qmlproperty int ClimateZone::steeringWheelHeater
 */
/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::steeringWheelHeater}

    \e value holds the steering wheel heater level, where the level can be between \c minimumValue(off)
    to \c maximumValue (warmest).
 */
/*!
   \property QtIVIClimateControl::steeringWheelHeater

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current steering wheel heater level.

   \sa AttributeSystem
   \sa steeringWheelHeater() setSteeringWheelHeater() steeringWheelHeaterAttribute()
 */
/*!
 * Returns the current steering wheel heater level, where the level can be between \c 0 (off) and \c 10 (warmest).
 *
 * \sa setSteeringWheelHeater() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
int QtIVIClimateControl::steeringWheelHeater() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_steeringWheelHeater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the steering wheel heater property.
 *
 * \sa setSteeringWheelHeater() steeringWheelHeater() steeringWheelHeaterChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::steeringWheelHeaterAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_steeringWheelHeaterAttribute;
}

QtIVIProperty *QtIVIClimateControl::steeringWheelHeaterProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_steeringWheelHeaterProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::fanSpeedLevel}

    \e value holds the fan speed level, where the level can be between \c minimumValue(off)
    to \c maximumValue (strongest).
 */
/*!
   \property QtIVIClimateControl::fanSpeedLevel

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current fan speed level.

   \sa AttributeSystem
   \sa fanSpeedLevel() setFanSpeedLevel() fanSpeedLevelAttribute()
 */
/*!
 * Returns the current fan speed level, where the level can be between \c 0 (off) and \c 10 (strongest).
 *
 * \sa setFanSpeedLevel() fanSpeedLevelChanged() fanSpeedLevelAttribute()
 */
int QtIVIClimateControl::fanSpeedLevel() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_fanSpeedLevel;
}

/*!
 * Returns the attribute defining the boundaries and availability of the fan speed level property
 *
 * \sa setFanSpeedLevel() fanSpeedLevel() fanSpeedLevelChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::fanSpeedLevelAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_fanSpeedLevelAttribute;
}

QtIVIProperty *QtIVIClimateControl::fanSpeedLevelProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_fanSpeedLevelProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::targetTemperature}

   \e value holds the target temperature of the zone expressed in centigrades, where the temperature can be between \c minimumValue(off)
   to \c maximumValue (strongest).
 */
/*!
   \property QtIVIClimateControl::targetTemperature

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current target temperature.

   \sa AttributeSystem
   \sa targetTemperature() setTargetTemperature() targetTemperatureAttribute()
 */
/*!
 * Returns the current target temperature expressed in centigrates.
 *
 * \sa setTargetTemperature() targetTemperatureChanged() targetTemperatureAttribute()
 */
int QtIVIClimateControl::targetTemperature() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_targetTemperature;
}

/*!
 * Returns the attribute defining the boundaries and availability of the target temperature property.
 *
 * \sa setTargetTemperature() targetTemperature() targetTemperatureChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::targetTemperatureAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_targetTemperatureAttribute;
}

QtIVIProperty *QtIVIClimateControl::targetTemperatureProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_targetTemperatureProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::seatCooler}

    \e value holds the seat cooler level, where the level can be between \c minimumValue(off)
    to \c maximumValue (coolest).
 */
/*!
   \property QtIVIClimateControl::seatCooler

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current seat cooler level.

   \sa AttributeSystem
   \sa seatCooler() setSeatCooler() seatCoolerAttribute()
 */
/*!
 * Returns the current seat cooler level, where the level can be between \c 0 (off) and \c 10 (coolest).
 *
 * \sa seatCooler() seatCoolerChanged() seatCoolerAttribute()
 */
int QtIVIClimateControl::seatCooler() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatCooler;
}

/*!
 * Returns the attribute defining the boundaries and availability of the seat cooler property.
 *
 * \sa setSeatCooler() seatCooler() seatCoolerChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::seatCoolerAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatCoolerAttribute;
}

QtIVIProperty *QtIVIClimateControl::seatCoolerProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatCoolerProperty;
}

/*!
   \qmlqtiviproperty {int} {QtIVIVehicleFunctions::ClimateControl::seatHeater}

    \e value holds the seat heater level, where the level can be between \c minimumValue(off)
    to \c maximumValue (warmest).
 */
/*!
   \property QtIVIClimateControl::seatHeater

   Holds a QtIVIProperty of type \e int where the QtIVIProperty::value() function returns the current seat heater level.

   \sa AttributeSystem
   \sa seatHeater() setSeatHeater() seatHeaterAttribute()
 */
/*!
 * Returns the current seat heater level, where the level can be between \c 0 (off) and \c 10 (warmest).
 *
 * \sa seatHeater() seatHeaterChanged() seatHeaterAttribute()
 */
int QtIVIClimateControl::seatHeater() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatHeater;
}

/*!
 * Returns the attribute defining the boundaries and availability of the seat heater property.
 *
 * \sa setSeatHeater() seatHeater() seatHeaterChanged()
 */
QtIVIPropertyAttribute<int> QtIVIClimateControl::seatHeaterAttribute() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatHeaterAttribute;
}

QtIVIProperty *QtIVIClimateControl::seatHeaterProperty() const
{
    Q_D(const QtIVIClimateControl);
    return d->m_seatHeaterProperty;
}

/*!
 * Sets the air conditioning system enabled, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isAirConditioningEnabled() airConditioningEnabledChanged() airConditioningAttribute()
 */
void QtIVIClimateControl::setAirConditioningEnabled(bool enabled)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setAirConditioningEnabled(enabled, zone());
}

/*!
 * Sets the air flow direction to \a direction.
 *
 * \sa airflowDirections() airflowDirectionsChanged() airflowDirectionsAttribute()
 */
void QtIVIClimateControl::setAirflowDirections(QtIVIClimateControl::AirflowDirections direction)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setAirflowDirections(direction, zone());
}

/*!
 * Enables the heater, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isHeaterEnabled() heaterEnabledChanged() heaterAttribute()
 */
void QtIVIClimateControl::setHeaterEnabled(bool enabled)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setHeaterEnabled(enabled, zone());
}

/*!
 * Sets the air recirculation system enabled, if \a enabled is true, otherwise it will be disabled.
 *
 * \sa isAirRecirculationEnabled() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
void QtIVIClimateControl::setAirRecirculationEnabled(bool enabled)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setAirRecirculationEnabled(enabled, zone());
}

/*!
 * Sets the current steering wheel heater level to \a value, where the level can be between \c 0 (off) and \c 10 (warmest).
 *
 * \sa steeringWheelHeater() steeringWheelHeaterChanged() steeringWheelHeaterAttribute()
 */
void QtIVIClimateControl::setSteeringWheelHeater(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setSteeringWheelHeater(value, zone());
}

/*!
 * Sets the current fan speed level to \a value, where the level can be between \c 0 (off) and \c 10 (strongest).
 *
 * \sa fanSpeedLevel() fanSpeedLevelChanged() fanSpeedLevelAttribute()
 */
void QtIVIClimateControl::setFanSpeedLevel(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setFanSpeedLevel(value, zone());
}

/*!
 * Sets the current target temperature to \a temperature expressed in centigrades.
 *
 * \sa targetTemperature() targetTemperatureChanged() targetTemperatureAttribute()
 */
void QtIVIClimateControl::setTargetTemperature(int temperature)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setTargetTemperature(temperature, zone());
}

/*!
 * Sets the current seat cooler level to \a value, where the level can be between \c 0 (off) and \c 10 (coolest).
 *
 * \sa seatCooler() seatCoolerChanged() seatCoolerAttribute()
 */
void QtIVIClimateControl::setSeatCooler(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setSeatCooler(value, zone());
}

/*!
 * Sets the current seat heater level to \a value, where the level can be between \c 0 (off) and \c 10 (warmest).
 *
 * \sa seatHeater() seatHeaterChanged() seatHeaterAttribute()
 */
void QtIVIClimateControl::setSeatHeater(int value)
{
    Q_D(QtIVIClimateControl);
    if (QtIVIClimateControlBackendInterface* backend = d->climateControlBackend())
        backend->setSeatHeater(value, zone());
}

/*!
 * \fn void QtIVIClimateControl::airflowDirectionsChanged(QtIVIClimateControl::AirflowDirections value)
 *
 * This signal is emitted whenever the air flow directions change. The new flow directions are passed as \a value.
 *
 * \sa airflowDirections() setAirflowDirections() airflowDirectionsAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::airflowDirectionsAttributeChanged(const QtIVIPropertyAttribute<QtIVIClimateControl::AirflowDirections> &attribute);
 *
 * This signal is emitted whenever the attribute for the airflowDirections property changes. The new attribute is passed as \a attribute.
 *
 * \sa airflowDirectionsAttribute() airflowDirections()
 */
/*!
 * \fn void QtIVIClimateControl::airConditioningEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the air conditioning system is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isAirConditioningEnabled() setAirConditioningEnabled() airConditioningAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::airConditioningAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the airConditioning property changes. The new attribute is passed as \a attribute.
 *
 * \sa airConditioningAttribute() isAirConditioningEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::heaterEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the heater is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isHeaterEnabled() setHeaterEnabled() heaterAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::airRecirculationEnabledChanged(bool enabled);
 *
 * This signal is emitted whenever the air recirculation system is turned \e on or \e off. The new value is passed as \a enabled.
 *
 * \sa isAirRecirculationEnabled() setAirRecirculationEnabled() airRecirculationAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::airRecirculationAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
 *
 * This signal is emitted whenever the attribute for the airRecirculation property changes. The new attribute is passed as \a attribute.
 *
 * \sa airRecirculationAttribute() isAirRecirculationEnabled()
 */
/*!
 * \fn void QtIVIClimateControl::steeringWheelHeaterChanged(int value);
 *
 * This signal is emitted whenever the steering wheel heater level changed. The new level is passed as \a value.
 *
 * \sa steeringWheelHeater() setSteeringWheelHeater() steeringWheelHeaterAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::steeringWheelHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the steeringWheelHeater property changes. The new attribute is passed as \a attribute.
 *
 * \sa steeringWheelHeaterAttribute() steeringWheelHeater()
 */
/*!
 * \fn void QtIVIClimateControl::fanSpeedLevelChanged(int value);
 *
 * This signal is emitted whenever the fan speed level changed. The new level is passed as \a value.
 *
 * \sa fanSpeedLevel() setFanSpeedLevel() fanSpeedLevelAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::fanSpeedLevelAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the fanSpeedLevel property changes. The new attribute is passed as \a attribute.
 *
 * \sa fanSpeedLevelAttribute() fanSpeedLevel()
 */
/*!
 * \fn void QtIVIClimateControl::targetTemperatureChanged(int temperature);
 *
 * This signal is emitted whenever the target temperature changed. The new temperature is passed as \a temperature.
 *
 * \sa targetTemperature() setTargetTemperature() targetTemperatureAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::targetTemperatureAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the targetTemperature property changes. The new attribute is passed as \a attribute.
 *
 * \sa targetTemperatureAttribute() targetTemperature()
 */
/*!
 * \fn void QtIVIClimateControl::seatCoolerChanged(int value);
 *
 * This signal is emitted whenever the seat cooler level changed. The new level is passed as \a value.
 *
 * \sa seatCooler() setSeatCooler() seatCoolerAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::seatCoolerAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the seatCooler property changes. The new attribute is passed as \a attribute.
 *
 * \sa seatCoolerAttribute() seatCooler()
 */
/*!
 * \fn void QtIVIClimateControl::seatHeaterChanged(int value);
 *
 * This signal is emitted whenever the seat heater level changed. The new level is passed as \a value.
 *
 * \sa seatHeater() setSeatHeater() seatHeaterAttribute()
 */
/*!
 * \fn void QtIVIClimateControl::seatHeaterAttributeChanged(const QtIVIPropertyAttribute<int> &attribute);
 *
 * This signal is emitted whenever the attribute for the heater property changes. The new attribute is passed as \a attribute.
 *
 * \sa heaterAttribute() isHeaterEnabled()
 */

 #include "moc_qtiviclimatecontrol.cpp"
