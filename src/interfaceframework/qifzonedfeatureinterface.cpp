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

    \brief QIfZonedFeatureInterface defines the base interface for all
    QIfAbstractZonedFeature derived classes.

    Implementing QIfZonedFeatureInterface follows the same pattern as described in
    QIfFeatureInterface, but to support zones some additional functions need to be implemented: all
    setters and signals which are zoned need to be adapted like described below.

    \section2 Providing Available Zones

    Before making any further calls to the backend, The zoned feature will query
    the list of available zones. Zones are string keys and can be anything
    defined by the backend developer. In this case we have two zones: "Left"
    and "Right":

    \code
    QStringList Backend::availableZones() const {
        QStringList zones;
        zones << "Left";
        zones << "Right";
        return zones;
    }
    \endcode

    \section2 Signal Definition

    A zoned signal or a change signal for a zoned property needs to provide the zone as the second
    parameter. Here an example for a zoned signal:

    \code
    void fanSpeedLevelChanged(int value, const QString &zone);
    \endcode

    Similar to implementing a QIfFeatureInterface, the backend needs to emit all property change
    signals with the initial values in the \l initialize() function.
    In a zoned backend, the implementation has to emit all supported property signals, passing the
    zone as a parameter. The zone parameter can be omitted if the property is unzoned.

    \code
    void Backend::initialize() {
        emit fanSpeedLevelChanged(2, "Left");
        emit fanSpeedLevelChanged(2, "Right");
        emit steeringWheelHeaterChanged(0); // Generic, no zone specified
        emit initializationDone();
    }
    \endcode

    \section2 Setter implementation

    A setter for a zoned property takes the zone as the second parameter after the property value.
    Validating the requested zone is the responsibility of the backend implementation and in case
    an unsupported zone is passed, a \l QIfAbstractFeature::InvalidZone error should be reported.

    In the example, the \e fanSpeedLevel property is zoned. If \e zone is valid, the actual fan
    speed level is adjusted and afterwards the change signal is emitted to indicate the change to
    the feature.
    \code
    void Backend::setFanSpeedLevel(int value, const QString &zone) {
        if (!m_fanSpeedZones.contains(zone)) {
            emit errorChanged(QIfAbstractFeature::InvalidZone);
        } else {
            // Set specified zone fan to value
            ...
            emit fanSpeedLevelChanged(value, zone);
        }
    }
    \endcode

    \sa QIfAbstractZonedFeature
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

    Use this signal when the list of available \a zones need to be retrieved first, and are not
    available when the backend instance is created.

    \sa availableZones() {Providing Available Zones}
*/

QT_END_NAMESPACE

#include "moc_qifzonedfeatureinterface.cpp"
