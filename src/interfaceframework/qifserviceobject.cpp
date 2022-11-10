// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
    QString id = QIfServiceInterface::id();
    if (!id.isEmpty())
        return id;

    return m_id;
}

const QVariantMap &QIfServiceObject::serviceSettings() const
{
    return m_serviceSettings;
}

void QIfServiceObject::updateServiceSettings(const QVariantMap &settings)
{
    if (m_serviceSettings == settings)
        return;
    m_serviceSettings = settings;
    emit serviceSettingsChanged();
}

QT_END_NAMESPACE
