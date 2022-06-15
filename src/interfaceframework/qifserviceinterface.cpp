// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifserviceinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \fn template <class T> T qif_interface_cast(QObject *backend)
    \relates QIfServiceInterface

    Casts the given \a backend to the interface type T.

    When implementing a QtInterfaceFramework Feature, use this function as it shows a meaningful error message
    when the cast fails.

    \sa qobject_cast
*/

/*!
    \class QIfServiceInterface
    \inmodule QtInterfaceFramework
    \brief Defines the interface for services.

    The QIfServiceInterface class defines the interface for services registered with QIfServiceManager.

    Typically, service objects inherit the concrete class, QIfServiceObject, instead of using
    QIfServiceInterface directly.

    \sa QIfServiceObject
*/

/*!
    Destructs the QIfServiceInterface instance.
*/
QIfServiceInterface::~QIfServiceInterface()
{
}

/*!
    \fn QStringList QIfServiceInterface::interfaces() const

    Returns a list of service interface names supported by the service object instance.
*/

/*!
    \fn QObject* QIfServiceInterface::interfaceInstance(const QString& interfaceName) const

    Returns an object implementing the service interface requested through \a interfaceName.
*/

QT_END_NAMESPACE
