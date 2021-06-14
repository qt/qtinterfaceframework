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

#include "qifproxyserviceobject.h"
#include "qifproxyserviceobject_p.h"

QT_BEGIN_NAMESPACE

QIfProxyServiceObjectPrivate::QIfProxyServiceObjectPrivate(QIfServiceInterface *interface)
    : m_serviceInterface(interface)
{
}

QIfProxyServiceObjectPrivate::QIfProxyServiceObjectPrivate(const QHash<QString, QIfFeatureInterface*> &interfaceMap)
    : m_serviceInterface(nullptr)
    , m_interfaceMap(interfaceMap)
{
}

/*!
    \class QIfProxyServiceObject
    \inmodule QtInterfaceFramework
    \brief QIfProxyServiceObject is a helper class to connect a Feature to already instantiated
    QIfServiceInterface classes.

    In constrast to the QIfServiceObject which is created for you by the QIfServiceManager for
    every backend plugin, the QIfProxyServiceObject can be instantiated by the user and then
    manually set to the feature class.

    There are two ways to construct a QIfProxyServiceObject. The first takes a
    QIfServiceInterface pointer as input and can be used to load a backend which is derived from
    QIfServiceInterface and supposed to be loaded as a plugin, but is part of the same library and
    can be loaded directly instead. e.g. within a autotest.

    The second constructor takes a QHash<QString, QIfFeatureInterface*> and can be used to directly
    connect a feature class to the backend implementing the QIfFeatureInterface.

    \sa QIfAbstractFeature
*/

/*!
    Creates a new QIfProxyServiceObject for the given \a interface.

    This can be used to load a backend which is derived from QIfServiceInterface and supposed to
    be loaded as a plugin, but is part of the same library and can be loaded directly instead. e.g.
    within a autotest
*/
QIfProxyServiceObject::QIfProxyServiceObject(QIfServiceInterface *interface)
    : QIfServiceObject()
    , d_ptr(new QIfProxyServiceObjectPrivate(interface))
{
}

/*!
    Creates a new QIfProxyServiceObject for the given \a interfaceMap.

    This can be used to directly connect a feature class to the backend implementing the
    QIfFeatureInterface.
*/
QIfProxyServiceObject::QIfProxyServiceObject(const QHash<QString, QIfFeatureInterface*> &interfaceMap)
    : QIfServiceObject()
    , d_ptr(new QIfProxyServiceObjectPrivate(interfaceMap))
{
}

/*!
    \reimp
*/
QStringList QIfProxyServiceObject::interfaces() const
{
    Q_D(const QIfProxyServiceObject);
    if (d->m_serviceInterface)
        return d->m_serviceInterface->interfaces();

    return d->m_interfaceMap.keys();
}

/*!
    \reimp
*/
QIfFeatureInterface *QIfProxyServiceObject::interfaceInstance(const QString &interface) const
{
    Q_D(const QIfProxyServiceObject);
    if (d->m_serviceInterface)
        return d->m_serviceInterface->interfaceInstance(interface);

    return d->m_interfaceMap.value(interface);
}

QT_END_NAMESPACE
