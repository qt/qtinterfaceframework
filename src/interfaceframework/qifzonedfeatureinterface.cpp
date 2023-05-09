// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifzonedfeatureinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfZonedFeatureInterface
    \inmodule QtInterfaceFramework
    \ingroup backends

    \brief QIfZonedFeatureInterface defines the base interface for the
    feature backend classes.

    Vehicle feature can be zoned or be just generic depending of the vehicle.
    For example some vehicles can contain only one climate fan and some other
    may have one fan for the front seats and one for the back seat. To fill both
    requirements, a backend developer can specify each feature attribute to be
    generic or zone specific. If the backend doesn't specify a zone for an
    attribute, then the attribute is generic. There can be multiple zone
    attributes, but only a single generic one. The zone value for a generic
    attribute is an empty string, and it can be omitted from the signals. The
    code snippets below describes how to implement zone specific fanSpeedlevel
    and generic steeringWheelHeater support in the backend.

    \section2 Providing Available Zones

    Before making any further calls to the backend, VehicleFunctions will query
    the list of available zones. Zones are string keys and can be anything
    defined by the backend developer. In this case we have two zones: "Front"
    and "Rear".

    The backend must return all available zones via
    \l {QIfZonedFeatureInterface::}{availableZones}:
    \code
    QStringList backend::availableZones() const {
        QStringList zones;
        zones << "Front";
        zones << "Rear";
        return zones;
   }
    \endcode

    \section2 Initializing Attributes

    VehicleFunctions calls the backend to initialize all attributes. Backend
    implementation has to emit all supported attribute signals, passing the
    zone as a parameter. Zone is not needed if attribute is generic.

    Initialization signals are emitted in the
    \l {QIfZonedFeatureInterface::}{initialize}:
    \code
    void backend::initialize() {
        emit fanSpeedLevelChanged(2, "Front");
        emit fanSpeedLevelChanged(2, "Rear");
        emit steeringWheelHeaterChanged(0); // Generic, no zone specified
        emit initializationDone();
   }
    \endcode

    \section2 Implementing Feature-specific Attributes

    Fan speed is zoned, validating requested zones is the responsibility
    of the backend. If zone is valid, the vehicle's actual fan speed level
    can be adjusted. The backend has to emit a signal for the changed zone.
    \code
    void backend::setFanSpeedLevel(int value, const QString &zone) {
        if (!m_fanSpeedZones.contains(zone)) {
            emit errorChanged(QIfAbstractFeature::InvalidZone);
        } else {
            // Set specified zone fan to value
            ...
            emit fanSpeedLevelChanged(value, zone);
        }
   }

    int backend::fanSpeedLevel(const QString &zone) {
        if (!m_fanSpeedZones.contains(zone)) {
            emit errorChanged(QIfAbstractFeature::InvalidZone);
            return -1;
        } else {
            int value = ... // Get vehicle's zone fan Speed
            return value;
        }
   }
    \endcode

    Steering wheel heater is not zone specific, so zone attribute should be
    empty. If zone is empty, the vehicle's actual steering wheel heater can
    be controlled. The backend has to emit a signal for the changed value.
    Because the attribute is generic, zone is omitted from the signal.
    \code
    void backend::setSteeringWheelHeater(int value, const QString &zone) {
        if (!zone.isEmpty()) {  // zone must be empty for a generic attribute
            emit errorChanged(QIfAbstractFeature::InvalidZone);
            return;
        } else {
            // Set vehicle's steering wheel heater value
            ...
            emit steeringWheelHeaterChanged(value);
        }
   }

    int backend::steeringWheelHeater(const QString &zone) {
        if (!zone.isEmpty()) {  // zone must be empty for a generic attribute
            emit errorChanged(QIfAbstractFeature::InvalidZone);
            return -1;
        } else {
            int value = ... // Get vehicle's steering wheel heater value
            return value;
        }
   }
    \endcode

    To implement a backend plugin you need also to implement QIfServiceInterface from the QtInterfaceFramework module.

    See the full example backend implementation from \c {src/plugins/ifvehiclefunctions/climate_simulator}.
    \sa QIfAbstractZonedFeature, QIfServiceInterface
*/

/*!
    \fn QIfZonedFeatureInterface::QIfZonedFeatureInterface(QObject *parent = nullptr)

    Constructs a backend base interface.

    The \a parent is sent to the QObject constructor.
*/
QIfZonedFeatureInterface::QIfZonedFeatureInterface(QObject *parent)
    : QIfFeatureInterface(parent)
{
}

/*!
    \fn QStringList QIfZonedFeatureInterface::availableZones() const

    Returns a list of supported zone names. This is called from the client
    after having connected.

    The returned names must be valid QML property names, i.e. \c {[a-z_][A-Za-z0-9_]*}.

    \sa availableZonesChanged() {Providing Available Zones}
*/

/*!
    \fn void QIfZonedFeatureInterface::availableZonesChanged(const QStringList &zones)

    Emitted when the available zones changed.

    Use this signal when the list of available \a zones need to be retrieved first, and are not available
    when the backend instance is created.

    \sa availableZones() {Providing Available Zones}
*/

QT_END_NAMESPACE
