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
    \qmltype ServiceObject
    \instantiates QIfServiceObject
    \inqmlmodule QtInterfaceFramework

    \brief Provides the connection point to a Backend Service.

    ServiceObject provides you with a list of interfaces that the Backend implements.

    From QML the object is mainly used to assign it manually to a AbstractFeature derived object or
    to update the service settings.
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
    \qmlproperty string ServiceObject::id
    \brief A unique ID for the service object instance.

    Holds the unique ID of the service object
*/
/*!
    \property QIfServiceObject::id
    \brief A unique ID for the service object instance.

    Each service object has a unique ID. When subclassing, the id()
    function can be overloaded to modify how this ID is generated.
*/

/*!
    \qmlproperty string ServiceObject::configurationId
    \brief Holds the id to determine which configuration this service object belongs to.
    \since 6.5

    Once the id has been set, it is possible to change certain values using the
    \l InterfaceFrameworkConfiguration API.

    \note Values set in the matching \l InterfaceFrameworkConfiguration can override the initial
    values set during the component creation.

    \sa InterfaceFrameworkConfiguration
*/
/*!
    \property QIfServiceObject::configurationId
    \brief Holds the id to determine which configuration this service object belongs to.
    \since 6.5

    Once the id has been set, it is possible to change certain values using the
    \l QIfConfiguration API.

    \note Values set in the matching \l QIfConfiguration can override the initial values
    set during the component creation.

    \sa QIfConfiguration
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

/*!
    \qmlproperty variant ServiceObject::serviceSettings
    \brief The settings for the service object instance.
    \since 6.5

    The serviceSettings property contains a map of settings for the service object instance.

    \sa {Backend specific configuration option}
*/
/*!
    \property QIfServiceObject::serviceSettings
    \brief The settings for the service object instance.
    \since 6.5

    The serviceSettings property contains a map of settings for the service object instance.

    \sa {Backend specific configuration option}
*/
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

#include "moc_qifserviceobject.cpp"
