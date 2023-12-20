// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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

    The \a parent argument is sent to the QIfServiceObject constructor.

    This can be used to load a backend which is derived from QIfServiceInterface and supposed to
    be loaded as a plugin, but is part of the same library and can be loaded directly instead. e.g.
    within a autotest
*/
QIfProxyServiceObject::QIfProxyServiceObject(QIfServiceInterface *interface, QObject *parent)
    : QIfServiceObject(parent)
    , d_ptr(new QIfProxyServiceObjectPrivate(interface))
{
}

/*!
    Creates a new QIfProxyServiceObject for the given \a interfaceMap.

    This can be used to directly connect a feature class to the backend implementing the
    QIfFeatureInterface.
*/
QIfProxyServiceObject::QIfProxyServiceObject(const QHash<QString, QIfFeatureInterface*> &interfaceMap, QObject *parent)
    : QIfServiceObject(parent)
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

QString QIfProxyServiceObject::id() const
{
    Q_D(const QIfProxyServiceObject);
    QString id;
    if (d->m_serviceInterface)
         id = d->m_serviceInterface->id();

    if (id.isEmpty())
        id = QIfServiceObject::id();
    return id;
}

QString QIfProxyServiceObject::configurationId() const
{
    Q_D(const QIfProxyServiceObject);
    if (d->m_serviceInterface)
        return d->m_serviceInterface->configurationId();

    return QIfServiceObject::configurationId();
}

void QIfProxyServiceObject::updateServiceSettings(const QVariantMap &settings)
{
    Q_D(const QIfProxyServiceObject);
    if (d->m_serviceInterface)
        d->m_serviceInterface->updateServiceSettings(settings);

    //Always also call the base class to keep the property in sync
    QIfServiceObject::updateServiceSettings(settings);
}

QT_END_NAMESPACE

#include "moc_qifproxyserviceobject.cpp"
