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

#include "qifserviceobject.h"

#include <QUuid>

QT_BEGIN_NAMESPACE

/*!
    \class QIfServiceObject
    \inmodule QtInterfaceFramework
    \brief Provides the connection point to a Backend Service.

    QIfServiceObject provides you with a list of interfaces that the Backend implements.

    Use interfaceInstance() to obtain a QObject that implements this interface. The
    interface returned may contain signals that a Feature implementing this interface
    needs to be connected to.

    \sa QIfAbstractFeature
*/


/*!
    Constructor.

    \a parent is passed on to \l QObject.
*/
QIfServiceObject::QIfServiceObject(QObject *parent)
    : QObject(parent)
    , m_id(QUuid::createUuid().toString())
{
}

/*!
    \property QIfServiceObject::id
    \brief A unique ID for the service object instance.

    Each service object has a unique ID. When subclassing, the id()
    function can be overloaded to modify how this ID is generated.
*/

/*!
    The id() function can be overloaded to modify how the unique ID is generated,
    for use by this service object.

    By default, QUuid::createUuid() is used.
*/
QString QIfServiceObject::id() const
{
    return m_id;
}

QT_END_NAMESPACE
