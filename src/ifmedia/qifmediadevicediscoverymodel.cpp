// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifmediadevicediscoverymodel.h"
#include "qifmediadevicediscoverymodel_p.h"

#include <QtInterfaceFramework/QIfServiceObject>
#include <QtIfMedia/QIfMediaDevice>
#include <QtDebug>

QT_BEGIN_NAMESPACE

QIfMediaDeviceDiscoveryModelPrivate::QIfMediaDeviceDiscoveryModelPrivate(const QString &interface, QIfMediaDeviceDiscoveryModel *parent)
    : QIfAbstractFeatureListModelPrivate(interface, parent)
    , q_ptr(parent)
{
}

void QIfMediaDeviceDiscoveryModelPrivate::initialize()
{
    QIfAbstractFeatureListModelPrivate::initialize();
    Q_Q(QIfMediaDeviceDiscoveryModel);

    QObject::connect(q, &QAbstractListModel::rowsInserted,
                     q, &QIfMediaDeviceDiscoveryModel::countChanged);
    QObject::connect(q, &QAbstractListModel::rowsRemoved,
                     q, &QIfMediaDeviceDiscoveryModel::countChanged);
    QObject::connect(q, &QAbstractListModel::modelReset,
                     q, &QIfMediaDeviceDiscoveryModel::countChanged);
}

void QIfMediaDeviceDiscoveryModelPrivate::clearToDefaults()
{
    qDeleteAll(m_deviceList);
    m_deviceList.clear();
}

void QIfMediaDeviceDiscoveryModelPrivate::resetModel(const QList<QIfServiceObject*> &deviceList)
{
    Q_Q(QIfMediaDeviceDiscoveryModel);
    q->beginResetModel();
    m_deviceList.clear();
    m_deviceList = deviceList;
    q->endResetModel();
}

void QIfMediaDeviceDiscoveryModelPrivate::onDeviceAdded(QIfServiceObject *device)
{
    QIfMediaDevice *mdevice = qobject_cast<QIfMediaDevice*>(device);
    if (!mdevice) {
        qWarning() << "Couldn't cast added Device to QIfMediaDevice. Aborting here.";
        return;
    }

    Q_Q(QIfMediaDeviceDiscoveryModel);
    q->beginInsertRows(QModelIndex(), m_deviceList.count(), m_deviceList.count());
    m_deviceList += device;
    q->endInsertRows();

    emit q->deviceAdded(mdevice);
}

void QIfMediaDeviceDiscoveryModelPrivate::onDeviceRemoved(QIfServiceObject *device)
{
    QIfMediaDevice *mdevice = qobject_cast<QIfMediaDevice*>(device);
    if (!mdevice) {
        qWarning() << "Couldn't cast removed Device to QIfMediaDevice. Aborting here.";
        return;
    }

    Q_Q(QIfMediaDeviceDiscoveryModel);
    int index = m_deviceList.indexOf(device);
    if (index == -1) {
        qWarning() << "Failed to remove the Device. Couldn't find" << device << "in the list of available devices";
        return;
    }
    q->beginRemoveRows(QModelIndex(), index, index);
    m_deviceList.takeAt(index);
    q->endRemoveRows();

    emit q->deviceRemoved(mdevice);

    delete device;
}

QIfMediaDeviceDiscoveryModelBackendInterface *QIfMediaDeviceDiscoveryModelPrivate::discoveryBackend() const
{
    return backend<QIfMediaDeviceDiscoveryModelBackendInterface*>();
}

/*!
    \class QIfMediaDeviceDiscoveryModel
    \inmodule QtIfMedia
    \brief Provides a model for discovering media devices.

    The QIfMediaDeviceDiscoveryModel provides a way to query for available media devices and
    to get notified when new media devices get added or are removed.

    All devices listed here need to be a subclass of QIfMediaDevice.

    The QIfMediaDeviceDiscoveryModel expects a single backend to be available. It is recommended to use it
    with \l {QIfAbstractFeatureListModel::}{discoveryMode} set to \l QIfAbstractFeature::AutoDiscovery.
*/

/*!
    \qmltype MediaDeviceDiscoveryModel
    \instantiates QIfMediaDeviceDiscoveryModel
    \inqmlmodule QtInterfaceFramework.Media
    \inherits AbstractFeatureListModel
    \brief Provides a model for discovering media devices.

    The MediaDeviceDiscoveryModel provides a way to query for available media devices and
    to get notified when new media devices get added or are removed.

    All devices listed here need to be a subclass of MediaDevice.

    The following roles are available in this model:

    \table
    \header
        \li Role name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The name of the media device. E.g. The name of the connected USB-Thumbdrive/SDCard or a connected Ipod.
    \row
        \li \c type
        \li string
        \li The type of the media device. See \l SupportedMediaDevices for a detailed listing.
    \row
        \li \c serviceObject
        \li QIfMediaDevice
        \li The Media Device. This object be used as ServiceObject for other Features. E.g. The FilterAndBrowseModel.
    \endtable


    The MediaDeviceDiscoveryModel expects a single backend to be available. It is recommended to use it
    with \l {AbstractFeatureListModel::}{discoveryMode} set to AbstractFeature.AutoDiscovery.
*/

/*!
    \enum QIfMediaDeviceDiscoveryModel::Roles
    \value NameRole
          The name of the media device. E.g. The name of the connected USB-Thumbdrive/SDCard or a connected Ipod.
    \value TypeRole
          The type of the media device. See \l SupportedMediaDevices for a detailed listing.
    \value ServiceObjectRole
          A pointer to the media device itself. This pointer can be used as the ServiceObject for other Features. E.g. The QIfFilterAndBrowseModel.
*/

/*!
    Constructs a QIfMediaDeviceDiscoveryModel.

    The \a parent argument is passed on to the \l QIfAbstractFeatureListModel base class.
*/
QIfMediaDeviceDiscoveryModel::QIfMediaDeviceDiscoveryModel(QObject *parent)
    : QIfAbstractFeatureListModel(*new QIfMediaDeviceDiscoveryModelPrivate(QLatin1String(QIfMediaDeviceDiscovery_iid), this), parent)
{
}

/*!
    \qmlproperty int MediaDeviceDiscoveryModel::count
    \brief Holds the current number of rows in this model.
*/
/*!
    \property QIfMediaDeviceDiscoveryModel::count
    \brief Holds the current number of rows in this model.
*/
int QIfMediaDeviceDiscoveryModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIfMediaDeviceDiscoveryModel);
    if (parent.isValid())
        return 0;

    return d->m_deviceList.count();
}

/*!
    \reimp
*/
QVariant QIfMediaDeviceDiscoveryModel::data(const QModelIndex &index, int role) const
{
    Q_D(const QIfMediaDeviceDiscoveryModel);
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= d->m_deviceList.count() || row < 0)
        return QVariant();

    QIfMediaDevice *item = qobject_cast<QIfMediaDevice*>(d->m_deviceList.at(row));
    if (!item)
        return QVariant();

    switch (role) {
        case NameRole: return item->name();
        case TypeRole: return item->type();
        case ServiceObjectRole: return QVariant::fromValue(item);
    }

    return QVariant();
}

/*!
    \qmlmethod MediaDevice MediaDeviceDiscoveryModel::get(i)

    Returns the media devices at index \a i.

    \note The returned device is owned by the model and can be deleted at any time.
    If stored in a property or a var, this could lead to a dangling pointer.
*/
/*!
    Returns the media device at index \a i.

    \note The returned device is owned by the model and can be deleted at any time.
*/
QIfMediaDevice *QIfMediaDeviceDiscoveryModel::get(int i) const
{
    Q_D(const QIfMediaDeviceDiscoveryModel);
    if (i >= d->m_deviceList.count() || i < 0)
        return nullptr;

    QIfMediaDevice *item = qobject_cast<QIfMediaDevice*>(d->m_deviceList.at(i));
    return item;
}

/*!
    Returns the media device at index \a i.

    \note The returned device is owned by the model and can be deleted at any time.
*/
QIfMediaDevice *QIfMediaDeviceDiscoveryModel::at(int i) const
{
    return get(i);
}

/*!
    \reimp
*/
QHash<int, QByteArray> QIfMediaDeviceDiscoveryModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[ServiceObjectRole] = "serviceObject";
    }
    return roles;
}

/*!
    \internal
*/
QIfMediaDeviceDiscoveryModel::QIfMediaDeviceDiscoveryModel(QIfMediaDeviceDiscoveryModelPrivate &dd, QObject *parent)
    : QIfAbstractFeatureListModel(dd, parent)
{
}

/*!
    \reimp
*/
void QIfMediaDeviceDiscoveryModel::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject)
    Q_D(QIfMediaDeviceDiscoveryModel);

    QIfMediaDeviceDiscoveryModelBackendInterface *backend = d->discoveryBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIfMediaDeviceDiscoveryModelBackendInterface::availableDevices,
                            d, &QIfMediaDeviceDiscoveryModelPrivate::resetModel);
    QObjectPrivate::connect(backend, &QIfMediaDeviceDiscoveryModelBackendInterface::deviceAdded,
                            d, &QIfMediaDeviceDiscoveryModelPrivate::onDeviceAdded);
    QObjectPrivate::connect(backend, &QIfMediaDeviceDiscoveryModelBackendInterface::deviceRemoved,
                            d, &QIfMediaDeviceDiscoveryModelPrivate::onDeviceRemoved);

    QIfAbstractFeatureListModel::connectToServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIfMediaDeviceDiscoveryModel::clearServiceObject()
{
    Q_D(QIfMediaDeviceDiscoveryModel);
    d->clearToDefaults();
}

/*!
    \fn void QIfMediaDeviceDiscoveryModel::deviceAdded(QIfMediaDevice *device)

    This signal is emitted whenever a new media device got added. The new media device is passed as \a device.
*/

/*!
    \qmlsignal MediaDeviceDiscoveryModel::deviceAdded(MediaDevice device)

    This signal is emitted whenever a new media device got added. The new media device is passed as \a device.
*/

/*!
    \fn void QIfMediaDeviceDiscoveryModel::deviceRemoved(QIfMediaDevice *device)

    This signal is emitted whenever a media device got removed. The device which got removed is passed as \a device.
    Afterwards the device will be deleted.
*/

/*!
    \qmlsignal MediaDeviceDiscoveryModel::deviceRemoved(MediaDevice device)

    This signal is emitted whenever a media device got removed. The device which got removed is passed as \a device.
    Afterwards the device will be deleted.
*/

QT_END_NAMESPACE

#include "moc_qifmediadevicediscoverymodel.cpp"
