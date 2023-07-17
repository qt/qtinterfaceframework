// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtCore/qmetaobject.h>
#include <QtInterfaceFramework/qifserviceobject.h>

#include "qifabstractzonedfeature.h"
#include "qifabstractzonedfeature_p.h"
#include "qifzonedfeatureinterface.h"

QT_BEGIN_NAMESPACE

QIfAbstractZonedFeaturePrivate::QIfAbstractZonedFeaturePrivate(const QString &interface, const QString &zone, QIfAbstractFeature *parent)
    : QIfAbstractFeaturePrivate(interface, parent)
    , m_zone(zone)
{}

/*!
    \class QIfAbstractZonedFeature
    \inmodule QtInterfaceFramework

    \since 5.6

    \brief The QIfAbstractZonedFeature is the base class for all QtInterfaceFramework vehicle features.

    QIfAbstractZonedFeature contains all feature attributes and zone handling.
*/

/*!
    \qmltype AbstractZonedFeature
    \instantiates QIfAbstractZonedFeature
    \inqmlmodule QtInterfaceFramework 1.0
    \inherits AbstractFeature
    \brief The AbstractZonedFeature is not directly accessible. The QML type provides
    base QML properties for each QML Vehicle feature like zone and error access.
*/

/*!
    Constructs a vehicle feature with a specific \a interface and \a zone.

    If \a parent is of type QIfAbstractZonedFeature, then the created instance
    uses parent for the backend connection. Parent is connected to the
    backend and will forward updates between child features and the backend.

    \sa QIfAbstractFeature
*/
QIfAbstractZonedFeature::QIfAbstractZonedFeature(const QString &interface, const QString &zone, QObject *parent)
    : QIfAbstractFeature(*new QIfAbstractZonedFeaturePrivate(interface, zone, this), parent)
{
}

/*!
    \reimp
*/
bool QIfAbstractZonedFeature::acceptServiceObject(QIfServiceObject *serviceObject)
{
    if (auto *parentFeature = qobject_cast<QIfAbstractZonedFeature*>(parent()))
        return parentFeature->acceptServiceObject(serviceObject);
    else if (serviceObject)
        return serviceObject->interfaces().contains(interfaceName());
    return false;
}

/*!
    \reimp
*/
void QIfAbstractZonedFeature::connectToServiceObject(QIfServiceObject *serviceObject)
{
    QIfZonedFeatureInterface *backend = nullptr;
    if (auto *parentFeature = qobject_cast<QIfAbstractZonedFeature*>(parent()))
        backend = parentFeature->backend();
    else if (serviceObject)
        backend = qobject_cast<QIfZonedFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));

    connect(backend, &QIfZonedFeatureInterface::availableZonesChanged, this, &QIfAbstractZonedFeature::initializeZones);

    if (backend) {
        QStringList zones = backend->availableZones();
        initializeZones(zones);
    }

    QIfAbstractFeature::connectToServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIfAbstractZonedFeature::clearServiceObject()
{
    Q_D(QIfAbstractZonedFeature);
    qDeleteAll(d->m_zoneFeatures);
    d->m_zoneFeatures.clear();
    d->m_zoneFeatureList.clear();
    d->m_zoneFeatureMap.clear();
    emit availableZonesChanged(QStringList());
    emit zonesChanged();
}

/*!
    Returns a pointer to the backend \a interface.

    Returns the parent backend, if the parent is a QIfAbstractZonedFeature type.
    Returns zero if no backend is connected.
*/
QIfZonedFeatureInterface *QIfAbstractZonedFeature::backend(const QString &interface) const
{
    QString iface = interface;
    if (iface.isEmpty())
        iface = interfaceName();

    if (auto *parentFeature = qobject_cast<QIfAbstractZonedFeature*>(parent()))
        return parentFeature->backend();
    else if (QIfServiceObject *so = serviceObject())
        return so->interfaceInstance<QIfZonedFeatureInterface*>(iface);
    return nullptr;
}


/*!
    \fn virtual QIfAbstractZonedFeature *QIfAbstractZonedFeature::createZoneFeature(const QString &zone) = 0

    Create a new child feature for the given \a zone.

    Returns zero if the feature can't be created using the parameters specified.
*/

/*!
    \qmlproperty QString AbstractZonedFeature::zone

    \brief Name of the zone for this zoned feature.

    The zone can be given in the feature initialization. This property lets you control a single
    specific feature zone only.

    This property is only writable before the backend is connected. Once the backend is
    discovered and the component is verified to be valid, the zone is not writable anymore.
    It's not recommended to change the zone after initialization.

    \qml
    ClimateControl {
        zone: "FrontLeft"
        onAirConditioningChanged: {
            // Take action on front left A/C changes.
        }
   }
    \endqml
*/
/*!
    \property QIfAbstractZonedFeature::zone

    \brief Name of the zone of this zoned feature.

    The zone can be given in the feature initialization. With this property it's possible to
    control only a single specific feature zone.

    This property is writable only before the backend is connected. Once the backend is discovered
    and the component is verified to be valid, zone is not writable anymore.

    It's recommended to initialize the zone in the feature constructor:

    \code
    QIfClimateControl *climateControl = new QIfClimateControl("FrontLeft", this);
    climateControl->startAutoDiscovery();
    QString zone = climateControl->zone();
    \endcode

    After initialization, it's not recommended to change the zone.

*/
QString QIfAbstractZonedFeature::zone() const
{
    Q_D(const QIfAbstractZonedFeature);
    return d->m_zone;
}

void QIfAbstractZonedFeature::setZone(const QString &zone)
{
    Q_D(QIfAbstractZonedFeature);
    if (backend() || d->m_zone == zone)
        return;
    d->m_zone = zone;
    emit zoneChanged();
}

void QIfAbstractZonedFeature::initializeZones(const QStringList &zones)
{
    if (!backend() || !zone().isEmpty())
        return;

    Q_D(QIfAbstractZonedFeature);
    for (const QString &zone : zones) {
        QIfAbstractZonedFeature *f = zoneAt(zone);
        if (!f) {
            if (this->zone() == zone)
                f = this;
            else
                f = createZoneFeature(zone);
            if (f) {
                f->d_func()->m_serviceObject = d->m_serviceObject;
                d->m_zoneFeatures.append(f);
                d->m_zoneFeatureList.append(QVariant::fromValue(f));
                d->m_zoneFeatureMap.insert(f->zone(), QVariant::fromValue(f));
                emit availableZonesChanged(d->m_zoneFeatureMap.keys());
                emit zonesChanged();
            }
        }
    }
}

/*!
    \qmlproperty QStringList AbstractZonedFeature::availableZones

    Holds a list of the available zones.
*/
/*!
    \property QIfAbstractZonedFeature::availableZones

    Holds a list of the available zones.
*/
QStringList QIfAbstractZonedFeature::availableZones() const
{
    Q_D(const QIfAbstractZonedFeature);
    return d->m_zoneFeatureMap.keys();
}


/*!
    Returns the given \a zone instance for the feature.
*/
QIfAbstractZonedFeature *QIfAbstractZonedFeature::zoneAt(const QString &zone) const
{
    Q_D(const QIfAbstractZonedFeature);
    for (QIfAbstractZonedFeature *f : d->m_zoneFeatures)
        if (f->zone() == zone)
            return f;
    return nullptr;
}

/*!
    Returns all zone instances for the feature.
*/
QList<QIfAbstractZonedFeature*> QIfAbstractZonedFeature::zones() const
{
    Q_D(const QIfAbstractZonedFeature);
    return d->m_zoneFeatures;
}

QIfAbstractZonedFeature::QIfAbstractZonedFeature(QIfAbstractZonedFeaturePrivate &dd, QObject *parent)
    : QIfAbstractFeature(dd, parent)
{
}

/*!
    \qmlproperty QVariantMap AbstractZonedFeature::zoneAt

    Provides direct feature access to the given zone.

    \code
    feature.zoneAt.FrontLeft
    \endcode
*/
/*!
    \property QIfAbstractZonedFeature::zoneAt

    Provides direct feature access to the given zone.
*/
QVariantMap QIfAbstractZonedFeature::zoneFeatureMap() const
{
    Q_D(const QIfAbstractZonedFeature);
    return d->m_zoneFeatureMap;
}

/*!
    \qmlproperty QVariantList AbstractZonedFeature::zones

    Provides access to the feature zones model.

    \code
    model: feature.zones
    \endcode
*/
/*!
    \property QIfAbstractZonedFeature::zones

    Provides access to the feature zones model.
*/
QVariantList QIfAbstractZonedFeature::zoneFeatureList() const
{
    Q_D(const QIfAbstractZonedFeature);
    return d->m_zoneFeatureList;
}

QT_END_NAMESPACE

#include "moc_qifabstractzonedfeature.cpp"
