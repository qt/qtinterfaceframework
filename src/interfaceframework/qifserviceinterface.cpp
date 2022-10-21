/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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
****************************************************************************/

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
