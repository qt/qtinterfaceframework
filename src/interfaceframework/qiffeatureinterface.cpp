// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qiffeatureinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfFeatureInterface
    \inmodule QtInterfaceFramework
    \ingroup backends

    \brief QIfFeatureInterface defines the base class for all backends.

    To implement a backend for a specific feature, you need to derive from this class. There's a
    one-to-one mapping between a specialized feature API and its corresponding feature interface
    class.

    The feature interface class specifies which functions and signals need to be implemented by a
    backend to work with a specific feature.

    This base class contains the initialization logic and the generic error handling, which is
    common between all interfaces.

    A feature interface for coffee machine could look like this:
    \code
    class CoffeMachineInterface : public QIfFeatureInterface
    {
        void setTargetTemperature(int targetTemperature) = 0;
        ...

    signals:
        void currentTemperatureChanged(int currentTemperature);
        void targetTemperatureChanged(int targetTemperature);
        ...
    }
    \endcode

    A backend implementation providing the actual functionality needs to implement all pure-virtual
    functions and the logic to provide property updates and their initial state. Once the feature
    knows which backend implementation needs to be used, it will request the initial state of all
    properties by calling the \l initialize() function. The implementation needs to emit the changed
    signals with the initial values and once it is done, emit the \l initializationDone signal.

    In the coffee machine example the implementation could look like this:

    \code
    class CoffeMachineImplementation : public CoffeMachineInterface
    {
        void initialize()
        {
            emit currentTemperature(0);
            emit targetTemperature(0);
            emit initializationDone();
        }

        void setTargetTemperature(int targetTemperature)
        {
            if (m_targetTemperature == targetTemperature)
                return;

            // adjust the heating element to the new target temperature
            emit targetTemperatureChanged(targetTemperature)
        }
    }
    \endcode

    The implementation for \e setTargetTemperature() controls the internal heating element and once
    that is done, the change signal is emitted to inform the feature about the new state of the
    property.

    \sa QIfAbstractFeature
*/

QIfFeatureInterface::QIfFeatureInterface(QObject *parent)
    : QObject(parent)
{
}

/*!
    \fn void QIfFeatureInterface::initialize()

    Initializes the backend. This function is called after a feature connected to the backend.
    It is expected that this function will inform about the current state of the backend by
    emitting signals with the current status.

    The last signal which needs to be sent is the initializationDone() signal.

    \sa initializationDone()
*/

/*!
    \fn void QIfFeatureInterface::errorChanged(QIfAbstractFeature::Error error, const QString &message = QString())

    The signal is emitted when \a error occurs in the backend.
    Error \a message is optional.
*/

/*!
    \fn void QIfFeatureInterface::initializationDone()

    The signal is emitted  once the current backend state has been sent to the feature after
    a call to initialize()

    \sa initialize
*/

QT_END_NAMESPACE

#include "moc_qiffeatureinterface.cpp"
