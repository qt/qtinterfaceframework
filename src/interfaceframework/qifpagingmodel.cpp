// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifpagingmodel.h"
#include "qifpagingmodel_p.h"

#include "qifpagingmodelinterface.h"
#include "qifqmlconversion_helper.h"

#include <QDebug>
#include <QMetaObject>

QT_BEGIN_NAMESPACE

QIfPagingModelPrivate::QIfPagingModelPrivate(const QString &interface, QIfPagingModel *model)
    : QIfAbstractFeatureListModelPrivate(interface, model)
    , q_ptr(model)
    , m_capabilities(QtInterfaceFrameworkModule::NoExtras)
    , m_chunkSize(30)
    , m_moreAvailable(false)
    , m_identifier(QUuid::createUuid())
    , m_fetchMoreThreshold(10)
    , m_fetchedDataCount(0)
    , m_loadingType(QIfPagingModel::FetchMore)
{
    QtInterfaceFrameworkModule::registerTypes();
    qRegisterMetaType<QIfPagingModel::LoadingType>();
    qRegisterMetaType<QIfStandardItem>();
    qRegisterMetaType<QIfStandardItem>("QIfFilterAndBrowseModelItem");
}

QIfPagingModelPrivate::~QIfPagingModelPrivate()
{
}

void QIfPagingModelPrivate::initialize()
{
    QIfAbstractFeatureListModelPrivate::initialize();

    Q_Q(QIfPagingModel);
    q->setDiscoveryMode(QIfAbstractFeature::NoAutoDiscovery);

    QObject::connect(q, &QAbstractListModel::rowsInserted,
                     q, &QIfPagingModel::countChanged);
    QObject::connect(q, &QAbstractListModel::rowsRemoved,
                     q, &QIfPagingModel::countChanged);
    QObject::connect(q, &QAbstractListModel::modelReset,
                     q, &QIfPagingModel::countChanged);
    QObjectPrivate::connect(q, &QIfPagingModel::fetchMoreThresholdReached,
                            this, &QIfPagingModelPrivate::onFetchMoreThresholdReached);
}

void QIfPagingModelPrivate::onInitializationDone()
{
    Q_Q(QIfPagingModel);

    if (q->isInitialized())
        return;

    //Register this instance with the backend. The backend can initialize the internal structure now
    backend()->registerInstance(m_identifier);

    resetModel();
}

void QIfPagingModelPrivate::onCapabilitiesChanged(QUuid identifier, QtInterfaceFrameworkModule::ModelCapabilities capabilities)
{
    if (!identifier.isNull() && identifier != m_identifier)
        return;

    if (m_capabilities == capabilities)
        return;

    Q_Q(QIfPagingModel);
    m_capabilities = capabilities;
    emit q->capabilitiesChanged(capabilities);
}

void QIfPagingModelPrivate::onDataFetched(QUuid identifier, const QList<QVariant> &items, int start, bool moreAvailable)
{
    if (!identifier.isNull() && (!items.count() || identifier != m_identifier))
        return;

    Q_ASSERT(items.count() <= m_chunkSize);
    Q_ASSERT((start + items.count() - 1) / m_chunkSize == start / m_chunkSize);

    Q_Q(QIfPagingModel);
    m_moreAvailable = moreAvailable;

    if (m_loadingType == QIfPagingModel::FetchMore) {
        q->beginInsertRows(QModelIndex(), int(m_itemList.count()), int(m_itemList.count() + items.count() -1));
        m_itemList += items;
        m_fetchedDataCount = int(m_itemList.count());
        q->endInsertRows();
    } else {
        const int newSize = start + int(items.count());
        if (m_itemList.count() <  newSize || m_availableChunks.count() < newSize / m_chunkSize) {
            qWarning() << "countChanged signal needs to be emitted before the dataFetched signal";
            return;
        }

        m_fetchedDataCount = start + int(items.count());

        for (int i = 0; i < items.count(); i++)
            m_itemList.replace(start + i, items.at(i));

        m_availableChunks.setBit(start / m_chunkSize);

        emit q->dataChanged(q->index(start), q->index(start + int(items.count()) -1));
    }
}

void QIfPagingModelPrivate::onCountChanged(QUuid identifier, int new_length)
{
    if (m_loadingType != QIfPagingModel::DataChanged || (!identifier.isNull() && identifier != m_identifier) || m_itemList.count() == new_length)
        return;

    Q_Q(QIfPagingModel);
    q->beginInsertRows(QModelIndex(), int(m_itemList.count()), int(m_itemList.count() + new_length -1));
    for (int i = 0; i < new_length; i++)
        m_itemList.append(QVariant());
    q->endInsertRows();

    m_availableChunks.resize(new_length / m_chunkSize + 1);
}

void QIfPagingModelPrivate::onDataChanged(QUuid identifier, const QList<QVariant> &data, int start, int count)
{
    if (!identifier.isNull() && identifier != m_identifier)
        return;

    if (start < 0 || start > m_itemList.count()) {
        if (m_loadingType == QIfPagingModel::DataChanged)
            qWarning("The provided start argument is out of range. Please make sure to emit the countChanged() before emitting dataChanged()");
        return;
    }

    if (count < 0 || count > m_itemList.count() - start) {
        if (m_loadingType == QIfPagingModel::DataChanged)
            qWarning("The provided start argument is out of range. Please make sure to emit the countChanged() before emitting dataChanged()");
        return;
    }

    Q_Q(QIfPagingModel);

    //delta > 0 insert rows
    //delta < 0 remove rows
    int delta = int(data.count()) - count;
    //find data overlap for updates
    int updateCount = qMin(int(data.count()), count);
    int updateCountEnd = updateCount > 0 ? updateCount + 1 : 0;
    //range which is either added or removed
    int insertRemoveStart = start + updateCountEnd;
    int insertRemoveCount = qMax(int(data.count()), count) - updateCount;

    if (updateCount > 0) {
        for (int i = start, j=0; j < updateCount; i++, j++)
            m_itemList.replace(i, data.at(j));
        emit q->dataChanged(q->index(start), q->index(start + updateCount -1));
    }

    if (delta < 0) { //Remove
        q->beginRemoveRows(QModelIndex(), insertRemoveStart, insertRemoveStart + insertRemoveCount -1);
        for (int i = insertRemoveStart; i < insertRemoveStart + insertRemoveCount; i++)
            m_itemList.removeAt(i);
        q->endRemoveRows();
    } else if (delta > 0) { //Insert
        q->beginInsertRows(QModelIndex(), insertRemoveStart, insertRemoveStart + insertRemoveCount -1);
        for (int i = insertRemoveStart, j = updateCountEnd; i < insertRemoveStart + insertRemoveCount; i++, j++)
            m_itemList.insert(i, data.at(j));
        q->endInsertRows();
    }
}

void QIfPagingModelPrivate::onFetchMoreThresholdReached()
{
    Q_Q(QIfPagingModel);
    q->fetchMore(QModelIndex());
}

void QIfPagingModelPrivate::resetModel()
{
    Q_Q(QIfPagingModel);

    q->beginResetModel();
    m_itemList.clear();
    m_availableChunks.clear();
    m_fetchedDataCount = 0;
    //Setting this to true to let fetchMore do one first fetchcall.
    m_moreAvailable = true;
    q->endResetModel();

    q->fetchMore(QModelIndex());
}

void QIfPagingModelPrivate::fetchData(int startIndex)
{
    if (!backend())
        return;

    m_moreAvailable = false;
    const int start = startIndex >= 0 ? startIndex : m_fetchedDataCount;
    const int chunkIndex = start / m_chunkSize;
    if (chunkIndex < m_availableChunks.size())
        m_availableChunks.setBit(chunkIndex);
    backend()->fetchData(m_identifier, start, m_chunkSize);
}

void QIfPagingModelPrivate::clearToDefaults()
{
    Q_Q(QIfPagingModel);

    m_chunkSize = 30;
    emit q->chunkSizeChanged(m_chunkSize);
    m_moreAvailable = false;
    m_identifier = QUuid::createUuid();
    m_fetchMoreThreshold = 10;
    emit q->fetchMoreThresholdChanged(m_fetchMoreThreshold);
    m_fetchedDataCount = 0;
    m_loadingType = QIfPagingModel::FetchMore;
    emit q->loadingTypeChanged(m_loadingType);
    m_capabilities = QtInterfaceFrameworkModule::NoExtras;
    emit q->capabilitiesChanged(m_capabilities);
    m_itemList.clear();

    resetModel();
}

const QIfStandardItem *QIfPagingModelPrivate::itemAt(int i) const
{
    const QVariant &var = m_itemList.at(i);
    return qtif_gadgetFromVariant<QIfStandardItem>(q_ptr, var);
}

QIfPagingModelInterface *QIfPagingModelPrivate::backend() const
{
    return QIfAbstractFeatureListModelPrivate::backend<QIfPagingModelInterface*>();
}

/*!
    \class QIfPagingModel
    \inmodule QtInterfaceFramework
    \brief The QIfPagingModel is a generic model to load its data using the "Paging" aproach.

    The QIfPagingModel should be used directly or as a base class whenever a lot of data needs to be
    presented in a ListView.

    The model only fetches the data it really needs and can it can be configured how this can be done using
    the loadingType property.

    The backend filling the model with data needs to implement the QIfPagingModelInterface class.

    \section1 Setting it up
    The QIfPagingModel is using QtInterfaceFramework's \l {Dynamic Backend System} and is derived from QIfAbstractFeatureListModel.
    Other than most "QtInterfaceFramework Feature classes", the QIfPagingModel doesn't automatically connect to available backends.

    The easiest approach to set it up, is to connect to the same backend used by another feature. E.g. for connecting to the
    media backend, use the instance from the mediaplayer feature:
    \code
        QIfMediaPlayer *player = new QIfMediaPlayer();
        player->startAutoDiscovery();
        QIfPagingModel *model = new QIfPagingModel();
        model->setServiceObject(player->serviceObject());
    \endcode

    \section1 Loading Types

    Multiple loading types are supported, as the QIfPagingModel is made to work with asynchronous requests to
    fetch its data. The FetchMore loading type is the default and is using the \l{QAbstractItemModel::}{canFetchMore()}/\l{QAbstractItemModel::}{fetchMore()} functions of
    QAbstractItemModel to fetch new data once the view hits the end of the currently available data. As fetching can take
    some time, there is the fetchMoreThreshold property which controls how much in advance a new fetch should be started.

    The other loading type is DataChanged. In contrast to FetchMore, the complete model is pre-populated with empty rows
    and the actual data for a specific row is fetched the first time the data() function is called. Once the data is available,
    the dataChanged() signal will be triggered for this row and the view will start to render the new data.

    Please see the documentation of \l{QIfPagingModel::}{LoadingType} for more details on how the modes work and
    when they are suitable to use.

    See the \l{Models} section for more information about all models in QtInterfaceFramework.
*/

/*!
    \enum QIfPagingModel::LoadingType
    \value FetchMore
           This is the default and can be used if you don't know the final size of the list (e.g. a infinite list).
           The list will detect that it is near the end (fetchMoreThreshold) and then fetch the next chunk of data using canFetchMore and fetchMore.
           The drawback of this method is that you can't display a dynamic scroll-bar indicator which is resized depending on the content of the list,
           because the final size of the data is not known.
           The other problem could be fast scrolling, as the data might not arrive in-time and scrolling stops. This can be tweaked by the fetchMoreThreshold property.

    \value DataChanged
           For this loading type you need to know how many items are in the list, as dummy items are created and the user can already start scrolling even though the data is not yet ready to be displayed.
           Similar to FetchMore, the data is also loaded in chunks. You can safely use a scroll indicator here.
           The delegate needs to support this approach, as it doesn't have content when it's first created.
*/

/*!
    \enum QIfPagingModel::Roles
    \value NameRole
          The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
    \value TypeRole
          The type of the item. E.g. "artist", "track", "contact".
    \value ItemRole
          The item itself. This provides access to the properties which are type specific. E.g. the address of a contact.
    \omitvalue LastRole
*/

/*!
    \qmltype PagingModel
    \nativetype QIfPagingModel
    \inqmlmodule QtInterfaceFramework
    \inherits AbstractFeatureListModel
    \brief The PagingModel is a generic model to load its data using the "Paging" aproach.

    The PagingModel should be used directly or as a base class whenever a lot of data needs to be
    presented in a ListView.

    The model only fetches the data it really needs and can it can be configured how this can be done using
    the loadingType property.

    All rows in the model need to be subclassed from StandardItem.

    The following roles are available in this model:

    \table
    \header
        \li Role name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
    \row
        \li \c type
        \li string
        \li The type of the item. E.g. "artist", "track", "contact".
    \row
        \li \c item
        \li object
        \li The item itself. This provides access to the properties which are type specific. E.g. the address of a contact.
    \endtable

    \section1 Setting it up
    The PagingModel is using QtInterfaceFramework's \l {Dynamic Backend System} and is derived from QIfAbstractFeatureListModel.
    Other than most "QtInterfaceFramework Feature classes", the PagingModel doesn't automatically connect to available backends.

    The easiest approach to set it up, is to connect to the same backend used by another feature. E.g. for connecting to the
    media backend, use the instance from the mediaplayer feature:
    \qml
        Item {
            MediaPlayer {
                id: player
            }

            PagingModel {
                serviceObject: player.serviceObject
            }
        }
    \endqml

    \section1 Loading Types

    Multiple loading types are supported, as the PagingModel is made to work with asynchronous requests to
    fetch its data. The FetchMore loading type is the default and is using the \l{QAbstractItemModel::}{canFetchMore()}/\l{QAbstractItemModel::}{fetchMore()} functions of
    QAbstractItemModel to fetch new data once the view hits the end of the currently available data. As fetching can take
    some time, there is the fetchMoreThreshold property which controls how much in advance a new fetch should be started.

    The other loading type is DataChanged. In contrast to FetchMore, the complete model is pre-populated with empty rows
    and the actual data for a specific row is fetched the first time the data() function is called. Once the data is available,
    the dataChanged() signal will be triggered for this row and the view will start to render the new data.

    Please see the documentation of loadingType for more details on how the modes work and
    when they are suitable to use.

    See the \l{Models} section for more information about all models in QtInterfaceFramework.
*/

/*!
    Constructs a QIfPagingModel.

    The \a parent argument is passed on to the \l QIfAbstractFeatureListModel base class.
*/
QIfPagingModel::QIfPagingModel(QObject *parent)
    : QIfAbstractFeatureListModel(*new QIfPagingModelPrivate(QStringLiteral(QIfPagingModel_iid), this), parent)
{
}

/*!
    \qmlproperty enumeration PagingModel::capabilities
    \brief Holds the capabilities of the backend.

    The capabilities controls what the backend supports.
    It can be a combination of the following values:

    \value NoExtras
           The backend does only support the minimum feature set and is stateful.
    \value SupportsGetSize
           The backend can return the final number of items for a specific request. This makes it possible to support the QIfPagingModel::DataChanged loading
           type.
    \value SupportsFiltering
           The backend supports filtering of the content. QIfFilterAndBrowseModelInterface::availableContentTypesChanged() and QIfFilterAndBrowseModelInterface::queryIdentifiersChanged() will be used as input for the
           \l {Qt Interface Framework Query Language}.
    \value SupportsSorting
           The backend supports sorting of the content. QIfFilterAndBrowseModelInterface::availableContentTypesChanged() and QIfFilterAndBrowseModelInterface::queryIdentifiersChanged() will be used as input for the
           \l {Qt Interface Framework Query Language}.
    \value SupportsAndConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the AND conjunction.
    \value SupportsOrConjunction
           The backend supports handling multiple filters at the same time and these filters can be combined by using the OR conjunction.
    \value SupportsStatelessNavigation
           The backend is stateless and supports handling multiple instances of a QIfFilterAndBrowseModel requesting different data at the same time.
           E.g. One request for artists, sorted by name and another request for tracks. The backend has to consider that both request come from models which are
           currently visible at the same time.
    \value SupportsInsert
           The backend supports inserting new items at a given position.
    \value SupportsMove
           The backend supports moving items within the model.
    \value SupportsRemove
           The backend supports removing items from the model.
*/

/*!
    \property QIfPagingModel::capabilities
    \brief Holds the capabilities of the backend for the current content of the model.

    The capabilities controls what the current contentType supports. e.g. filtering or sorting.
*/

QtInterfaceFrameworkModule::ModelCapabilities QIfPagingModel::capabilities() const
{
    Q_D(const QIfPagingModel);
    return d->m_capabilities;
}

/*!
    \qmlproperty int PagingModel::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
*/

/*!
    \property QIfPagingModel::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
*/
int QIfPagingModel::chunkSize() const
{
    Q_D(const QIfPagingModel);
    return d->m_chunkSize;
}

void QIfPagingModel::setChunkSize(int chunkSize)
{
    Q_D(QIfPagingModel);
    if (d->m_chunkSize == chunkSize)
        return;

    d->m_chunkSize = chunkSize;
    emit chunkSizeChanged(chunkSize);
}

/*!
    \qmlproperty int PagingModel::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
*/

/*!
    \property QIfPagingModel::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
*/
int QIfPagingModel::fetchMoreThreshold() const
{
    Q_D(const QIfPagingModel);
    return d->m_fetchMoreThreshold;
}

void QIfPagingModel::setFetchMoreThreshold(int fetchMoreThreshold)
{
    Q_D(QIfPagingModel);
    if (d->m_fetchMoreThreshold == fetchMoreThreshold)
        return;

    d->m_fetchMoreThreshold = fetchMoreThreshold;
    emit fetchMoreThresholdChanged(fetchMoreThreshold);
}

/*!
    \qmlproperty enumeration PagingModel::loadingType
    \brief Holds the currently used loading type used for loading the data.

    It can be one of the following values:
    \target FetchMore
    \value FetchMore
           This is the default and can be used if you don't know the final size of the list (e.g. a infinite list).
           The list will detect that it is near the end (fetchMoreThreshold) and then fetch the next chunk of data using canFetchMore and fetchMore.
           The drawback of this method is that you can't display a dynamic scroll-bar indicator which is resized depending on the content of the list,
           because the final size of the data is not known.
           The other problem could be fast scrolling, as the data might not arrive in-time and scrolling stops. This can be tweaked by the fetchMoreThreshold property.

    \target DataChanged
    \value DataChanged
           For this loading type you need to know how many items are in the list, as dummy items are created and the user can already start scrolling even though the data is not yet ready to be displayed.
           Similar to FetchMore, the data is also loaded in chunks. You can safely use a scroll indicator here.
           The delegate needs to support this approach, as it doesn't have content when it's first created.

    \note When changing this property the content will be reset.
*/

/*!
    \property QIfPagingModel::loadingType
    \brief Holds the currently used loading type used for loading the data.

    \note When changing this property the content will be reset.
*/
QIfPagingModel::LoadingType QIfPagingModel::loadingType() const
{
    Q_D(const QIfPagingModel);
    return d->m_loadingType;
}

void QIfPagingModel::setLoadingType(QIfPagingModel::LoadingType loadingType)
{
    Q_D(QIfPagingModel);
    if (d->m_loadingType == loadingType)
        return;

    if (loadingType == QIfPagingModel::DataChanged && !d->m_capabilities.testFlag(QtInterfaceFrameworkModule::SupportsGetSize)) {
        qtif_qmlOrCppWarning(this, "The backend doesn't support the DataChanged loading type. This call will have no effect");
        return;
    }

    d->m_loadingType = loadingType;
    emit loadingTypeChanged(loadingType);

    d->resetModel();
}

/*!
    \qmlproperty int PagingModel::count
    \brief Holds the current number of rows in this model.
*/
/*!
    \property QIfPagingModel::count
    \brief Holds the current number of rows in this model.
*/
int QIfPagingModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIfPagingModel);
    if (parent.isValid())
        return 0;

    return int(d->m_itemList.count());
}

/*!
    \reimp
*/
QVariant QIfPagingModel::data(const QModelIndex &index, int role) const
{
    Q_D(const QIfPagingModel);
    Q_UNUSED(role)
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= d->m_itemList.count() || row < 0)
        return QVariant();

    const int chunkIndex = row / d->m_chunkSize;
    if (d->m_loadingType == DataChanged && !d->m_availableChunks.at(chunkIndex)) {
        //qWarning() << "Cache miss: Fetching Data for index " << row << "and following";
        const_cast<QIfPagingModelPrivate*>(d)->fetchData(chunkIndex * d->m_chunkSize);
        return QVariant();
    }

    if (row >= d->m_fetchedDataCount - d->m_fetchMoreThreshold && canFetchMore(QModelIndex()))
        emit const_cast<QIfPagingModel*>(this)->fetchMoreThresholdReached();

    const QIfStandardItem *item = d->itemAt(row);
    if (!item) {
        //qWarning() << "Cache miss: Waiting for fetched Data";
        return QVariant();
    }

    switch (role) {
    case NameRole: return item->name();
    case TypeRole: return item->type();
    case ItemRole: return d->m_itemList.at(row);
    }

    return QVariant();
}

/*!
    \fn template <typename T> T QIfPagingModel::at(int i) const

    Returns the item at index \a i converted to the template type T.
*/
/*!
    \qmlmethod object PagingModel::get(i)

    Returns the item at index \a i.
*/
/*!
    Returns the item at index \a i as QVariant.

    This function is intended to be used from QML. For C++
    please use the at() instead.
*/
QVariant QIfPagingModel::get(int i) const
{
    return data(index(i,0), ItemRole);
}

/*!
    \qmlmethod PagingModel::reload()

    Resets the model and starts fetching the content again.
*/
/*!
    Resets the model and starts fetching the content again.
*/
void QIfPagingModel::reload()
{
    Q_D(QIfPagingModel);
    d->resetModel();
}

/*!
    \reimp
*/
bool QIfPagingModel::canFetchMore(const QModelIndex &parent) const
{
    Q_D(const QIfPagingModel);
    if (parent.isValid())
        return false;

    return d->m_moreAvailable;
}

/*!
    \reimp
*/
void QIfPagingModel::fetchMore(const QModelIndex &parent)
{
    Q_D(QIfPagingModel);
    if (parent.isValid())
        return;

    if (!d->backend() || !d->m_moreAvailable)
        return;

    d->m_moreAvailable = false;
    d->fetchData(-1);
}

/*!
    \reimp
*/
QHash<int, QByteArray> QIfPagingModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[TypeRole] = "type";
        roles[ItemRole] = "item";
    }
    return roles;
}

/*!
    \internal
*/
QIfPagingModel::QIfPagingModel(QIfServiceObject *serviceObject, QObject *parent)
    : QIfPagingModel(parent)
{
    setServiceObject(serviceObject);
}

/*!
    \internal
*/
QIfPagingModel::QIfPagingModel(QIfPagingModelPrivate &dd, QObject *parent)
    : QIfAbstractFeatureListModel(dd, parent)
{
}

/*!
    \reimp
*/
void QIfPagingModel::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfPagingModel);

    QIfPagingModelInterface *backend = d->backend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIfPagingModelInterface::initializationDone,
                            d, &QIfPagingModelPrivate::onInitializationDone);
    QObjectPrivate::connect(backend, &QIfPagingModelInterface::supportedCapabilitiesChanged,
                            d, &QIfPagingModelPrivate::onCapabilitiesChanged);
    QObjectPrivate::connect(backend, &QIfPagingModelInterface::dataFetched,
                            d, &QIfPagingModelPrivate::onDataFetched);
    QObjectPrivate::connect(backend, &QIfPagingModelInterface::countChanged,
                            d, &QIfPagingModelPrivate::onCountChanged);
    QObjectPrivate::connect(backend, &QIfPagingModelInterface::dataChanged,
                            d, &QIfPagingModelPrivate::onDataChanged);

    QIfAbstractFeatureListModel::connectToServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIfPagingModel::disconnectFromServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfPagingModel);

    auto backend = d->backend();

    if (backend)
        backend->unregisterInstance(d->m_identifier);

    QIfAbstractFeatureListModel::disconnectFromServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIfPagingModel::clearServiceObject()
{
    Q_D(QIfPagingModel);
    d->clearToDefaults();
}

/*!
    \fn void QIfPagingModel::fetchMoreThresholdReached()

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

/*!
    \qmlsignal PagingModel::fetchMoreThresholdReached()

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

QT_END_NAMESPACE

#include "moc_qifpagingmodel.cpp"
