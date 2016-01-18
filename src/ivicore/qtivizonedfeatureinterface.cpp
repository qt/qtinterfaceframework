/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
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
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qtivizonedfeatureinterface.h"

/*!
   \class QtIVIZonedFeatureInterface
   \inmodule QtIVICore
   \ingroup backends

   \brief QtIVIZonedFeatureInterface defines the base interface for the
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
   \l {QtIVIZonedFeatureInterface::}{availableZones}:
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
   \l {QtIVIZonedFeatureInterface::}{initializeAttributes}:
   \code
   void backend::initializeAttributes() {
        emit fanSpeedLevelChanged(2, "Front");
        emit fanSpeedLevelChanged(2, "Rear");
        emit steeringWheelHeaterChanged(0); // Generic, no zone specified
   }
   \endcode

   \section2 Implementing Feature-specific Attributes

   Fan speed is zoned, validating requested zones is the responsibility
   of the backend. If zone is valid, the vehicle's actual fan speed level
   can be adjusted. The backend has to emit a signal for the changed zone.
   \code
   void backend::setFanSpeedLevel(int value, const QString &zone) {
        if (!m_fanSpeedZones.contains(zone)) {
            emit errorChanged(QtIVIAbstractFeature::InvalidZone);
        } else {
            // Set specified zone fan to value
            ...
            emit fanSpeedLevelChanged(value, zone);
        }
   }

   int backend::fanSpeedLevel(const QString &zone) {
        if (!m_fanSpeedZones.contains(zone)) {
            emit errorChanged(QtIVIAbstractFeature::InvalidZone);
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
            emit errorChanged(QtIVIAbstractFeature::InvalidZone);
            return;
        } else {
            // Set vehicle's steering wheel heater value
            ...
            emit steeringWheelHeaterChanged(value);
        }
   }

   int backend::steeringWheelHeater(const QString &zone) {
        if (!zone.isEmpty()) {  // zone must be empty for a generic attribute
            emit errorChanged(QtIVIAbstractFeature::InvalidZone);
            return -1;
        } else {
            int value = ... // Get vehicle's steering wheel heater value
            return value;
        }
   }
   \endcode

   To implement a backend plugin you need also to implement QtIVIServiceInterface from the QtIVICore module.

   See the full example backend implementation from \c {src/plugins/ivivehiclefunctions/climate_simulator}.
   \sa QtIVIAbstractZonedFeature, QtIVIServiceInterface
 */

/*!
 * \fn QtIVIZonedFeatureInterface::QtIVIZonedFeatureInterface(QObject *parent=0)
 *
 * Constructs a backend base interface.
 *
 * The \a parent is sent to the QObject constructor.
 */
QtIVIZonedFeatureInterface::QtIVIZonedFeatureInterface(QObject *parent)
    : QObject(parent)
{
}

/*!
 * \fn QStringList QtIVIZonedFeatureInterface::availableZones() const
 *
 * Returns a list of supported zone names. This is called from the client
 * right after it's connected.
 *
 * The returned names must be valid QML property names, i.e. \c {[a-z_][A-Za-z0-9_]*}.
 *
 * \sa {Providing Available Zones}
 */

/*!
 * \fn void QtIVIZonedFeatureInterface::initializeAttributes()
 *
 * Called from the client to initialize attributes. This is called after
 * client is connected and available zones are fetched.
 *
 * In this function all supported attributes for each zone need to be emitted with
 * the initialized value.
 *
 * \sa {Initializing Attributes}
 */

/*!
 * \fn void errorChanged(QtIVIAbstractFeature::Error error, const QString &message = QString())
 *
 * The signal is emitted when \a error occurs in the backend.
 * Error \a message is optional.
 */

