/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

#include "qiffeatureinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfFeatureInterface
    \inmodule QtInterfaceFramework
    \ingroup backends

    \brief QIfFeatureInterface defines the base class for all backends.

    To implement a backend for a specific feature you need to derive from this class.
    There's a one-to-one mapping between a spezialized feature API and its corresponding feature interface
    class.

    The feature interface class specifies which functions and signals need to be implemented by a backend
    to work with a specific feature.

    This base class contains the generic error handling, which is common between all interfaces.

    See the full example backend implementation from \c {src/plugins/ifvehiclefunctions/climate_simulator}.
    \sa QIfAbstractFeature, QIfServiceInterface
*/

QIfFeatureInterface::QIfFeatureInterface(QObject *parent)
    : QObject(parent)
{
}

QIfFeatureInterface::QIfFeatureInterface(QObjectPrivate &dd, QObject *parent)
    : QObject(dd, parent)
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
