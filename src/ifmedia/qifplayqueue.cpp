// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifplayqueue.h"
#include "qifplayqueue_p.h"
#include "qifmediaplayer.h"
#include "qifqmlconversion_helper.h"

#include <QtDebug>

QT_BEGIN_NAMESPACE

QIfPlayQueuePrivate::QIfPlayQueuePrivate(QIfMediaPlayer *player, QIfPlayQueue *model)
    : QAbstractItemModelPrivate()
    , q_ptr(model)
    , m_player(player)
    , m_identifier(QUuid::createUuid())
    , m_currentIndex(-1)
    , m_chunkSize(30)
    , m_moreAvailable(false)
    , m_fetchMoreThreshold(10)
    , m_fetchedDataCount(0)
    , m_canReportCount(false)
    , m_loadingType(QIfPlayQueue::FetchMore)
{
    qRegisterMetaType<QIfPlayableItem>();
}

QIfPlayQueuePrivate::~QIfPlayQueuePrivate()
{
}

void QIfPlayQueuePrivate::initialize()
{
    Q_Q(QIfPlayQueue);

    QObject::connect(q, &QAbstractListModel::rowsInserted,
                     q, &QIfPlayQueue::countChanged);
    QObject::connect(q, &QAbstractListModel::rowsRemoved,
                     q, &QIfPlayQueue::countChanged);
    QObject::connect(q, &QAbstractListModel::modelReset,
                     q, &QIfPlayQueue::countChanged);
    QObjectPrivate::connect(q, &QIfPlayQueue::fetchMoreThresholdReached,
                            this, &QIfPlayQueuePrivate::onFetchMoreThresholdReached);
}

void QIfPlayQueuePrivate::onInitializationDone()
{
    if (m_player->isInitialized())
        return;

    resetModel();
}

void QIfPlayQueuePrivate::onCurrentIndexChanged(int currentIndex)
{
    if (m_currentIndex == currentIndex)
        return;
    Q_Q(QIfPlayQueue);
    m_currentIndex = currentIndex;
    emit q->currentIndexChanged(currentIndex);
}

void QIfPlayQueuePrivate::onCanReportCountChanged(bool canReportCount)
{
    m_canReportCount = canReportCount;
}

void QIfPlayQueuePrivate::onDataFetched(const QUuid &identifier, const QList<QVariant> &items, int start, bool moreAvailable)
{
    if (m_identifier != identifier)
        return;

    if (!items.count())
        return;

    Q_Q(QIfPlayQueue);
    m_moreAvailable = moreAvailable;

    if (m_loadingType == QIfPlayQueue::FetchMore) {
        q->beginInsertRows(QModelIndex(), int(m_itemList.count()), int(m_itemList.count() + items.count() -1));
        m_itemList += items;
        m_fetchedDataCount = int(m_itemList.count());
        q->endInsertRows();
    } else {
        if (m_itemList.count() < start + items.count()) {
            qWarning() << "countChanged signal needs to be emitted before the dataFetched signal";
            return;
        }

        m_fetchedDataCount = start + int(items.count());

        for (int i = 0; i < items.count(); i++)
            m_itemList.replace(start + i, items.at(i));
        emit q->dataChanged(q->index(start), q->index(start + int(items.count()) -1));
    }
}

void QIfPlayQueuePrivate::onCountChanged(int new_length)
{
    if (m_loadingType != QIfPlayQueue::DataChanged || m_itemList.count() == new_length)
        return;

    Q_Q(QIfPlayQueue);
    q->beginInsertRows(QModelIndex(), int(m_itemList.count()), int(m_itemList.count() + new_length -1));
    for (int i = 0; i < new_length; i++)
        m_itemList.append(QVariant());
    q->endInsertRows();
}

void QIfPlayQueuePrivate::onDataChanged(const QList<QVariant> &data, int start, int count)
{
    if (start < 0 || start > m_itemList.count()) {
        if (m_loadingType == QIfPlayQueue::DataChanged)
            qWarning("The provided start argument is out of range. Please make sure to emit the countChanged() before emitting dataChanged()");
        return;
    }

    if (count < 0 || count > m_itemList.count() - start) {
        if (m_loadingType == QIfPlayQueue::DataChanged)
            qWarning("The provided start argument is out of range. Please make sure to emit the countChanged() before emitting dataChanged()");
        return;
    }

    Q_Q(QIfPlayQueue);

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

void QIfPlayQueuePrivate::onFetchMoreThresholdReached()
{
    Q_Q(QIfPlayQueue);
    q->fetchMore(QModelIndex());
}

void QIfPlayQueuePrivate::resetModel()
{
    Q_Q(QIfPlayQueue);
    q->beginResetModel();
    m_itemList.clear();
    q->endResetModel();
    m_fetchedDataCount = 0;

    m_moreAvailable = false;
    q->fetchMore(QModelIndex());
}

void QIfPlayQueuePrivate::clearToDefaults()
{
    Q_Q(QIfPlayQueue);
    m_identifier = QUuid::createUuid();
    m_currentIndex = -1;
    emit q->currentIndexChanged(-1);
    m_chunkSize = 30;
    emit q->chunkSizeChanged(30);
    m_moreAvailable = false;
    m_fetchMoreThreshold = 10;
    emit q->fetchMoreThresholdChanged(m_fetchMoreThreshold);
    m_loadingType = QIfPlayQueue::FetchMore;
    emit q->loadingTypeChanged(m_loadingType);

    resetModel();
}

const QIfPlayableItem *QIfPlayQueuePrivate::itemAt(int i) const
{
    QVariant var = m_itemList.at(i);
    if (!var.isValid())
        return nullptr;

    return qtif_gadgetFromVariant<QIfPlayableItem>(q_ptr, var);
}

QIfMediaPlayerBackendInterface *QIfPlayQueuePrivate::playerBackend() const
{
    return m_player->d_func()->playerBackend();
}

/*!
    \class QIfPlayQueue
    \inmodule QtIfMedia
    \brief Provides a play queue for the QIfMediaPlayer.

    The QIfPlayQueue is a model which is used by the QIfMediaPlayer to control the
    play order of QIfPlayableItems.

    It provides mechanisms for adding new items and managing the existing ones by removing
    or moving them around.

    The QIfPlayQueue can't be instantiated by its own and can only be retrieved through the QIfMediaPlayer.

    The following roles are available in this model:

    \table
    \header
        \li Role name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The name of the playable item. E.g. The track name or the name of the web-stream.
    \row
        \li \c type
        \li string
        \li The type of the playable item. E.g. \e "track" or \e "web-stream"
    \row
        \li \c item
        \li QIfPlayableItem
        \li The playable item instance. This can be used to access type specific properties like the artist.
    \endtable

*/

/*!
    \enum QIfPlayQueue::Roles
    \value NameRole
          The name of the playable item. E.g. The track name or the name of the web-stream.
    \value TypeRole
          The type of the playable item. E.g. \e "track" or \e "web-stream"
    \value ItemRole
          The playable item instance. This can be used to access type specific properties like the artist.
*/

/*!
    \enum QIfPlayQueue::LoadingType
    \value FetchMore
           This is the default and can be used if you don't know the final size of the list (e.g. a infinite list).
           The list will detect that it is near the end (fetchMoreThreshold) and then fetch the next chunk of data using canFetchMore and fetchMore.
           The drawback of this method is that, because the final size of the data is not known, you can't display a dynamic scroll-bar indicator which is resized depending on the content of the list.
           The other problem could be fast scrolling, as the data might not come in-time and scrolling stops. This can be tweaked by the fetchMoreThreshold property.

    \value DataChanged
           For this loading type you need to know how many items are in the list, as dummy items are created and the user can already start scrolling even though the data is not yet ready to be displayed.
           Similar to FetchMore the data is also loaded in chunks. You can safely use a scroll indicator here.
           The delegate needs to support this approach, as it doesn't have a content when it's first created.
*/

/*!
    \qmltype PlayQueue
    \instantiates QIfPlayQueue
    \inqmlmodule QtInterfaceFramework.Media
    \inherits QAbstractListModel
    \brief Provides a play queue for the MediaPlayer.

    The PlayQueue is a model which is used by the MediaPlayer to control the
    play order of PlayableItems.

    It provides mechanisms for adding new items and managing the existing ones by removing
    or moving them around.

    The PlayQueue can't be instantiated by its own and can only be retrieved through the MediaPlayer.
*/


QIfPlayQueue::~QIfPlayQueue()
{
}

/*!
    \qmlproperty int PlayQueue::currentIndex
    \brief Holds the index of the currently active track.

    Use the get() method to retrieve more information about the active track.
*/

/*!
    \property QIfPlayQueue::currentIndex
    \brief Holds the index of the currently active track.

    Use the get() method to retrieve more information about the active track.
*/
int QIfPlayQueue::currentIndex() const
{
    Q_D(const QIfPlayQueue);
    return d->m_currentIndex;
}

void QIfPlayQueue::setCurrentIndex(int currentIndex)
{
    Q_IF_BACKEND(QIfPlayQueue, d->playerBackend(), "Can't set the current index without a connected backend");

    backend->setCurrentIndex(currentIndex);
}

/*!
    \qmlproperty int PlayQueue::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
*/

/*!
    \property QIfPlayQueue::chunkSize
    \brief Holds the number of rows which are requested from the backend interface.

    This property can be used to fine tune the loading performance.

    Bigger chunks means less calls to the backend and to a potential IPC underneath, but more data
    to be transferred and probably longer waiting time until the request was finished.
*/
int QIfPlayQueue::chunkSize() const
{
    Q_D(const QIfPlayQueue);
    return d->m_chunkSize;
}

void QIfPlayQueue::setChunkSize(int chunkSize)
{
    Q_D(QIfPlayQueue);
    if (d->m_chunkSize == chunkSize)
        return;

    d->m_chunkSize = chunkSize;
    emit chunkSizeChanged(chunkSize);
}

/*!
    \qmlproperty int PlayQueue::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
*/

/*!
    \property QIfPlayQueue::fetchMoreThreshold
    \brief Holds the row delta to the end before the next chunk is loaded

    This property can be used to fine tune the loading performance. When the
    threshold is reached the next chunk of rows are requested from the backend. How many rows are fetched
    can be defined by using the chunkSize property.

    The threshold defines the number of rows before the cached rows ends.

    \note This property is only used when loadingType is set to FetchMore.
*/
int QIfPlayQueue::fetchMoreThreshold() const
{
    Q_D(const QIfPlayQueue);
    return d->m_fetchMoreThreshold;
}

void QIfPlayQueue::setFetchMoreThreshold(int fetchMoreThreshold)
{
    Q_D(QIfPlayQueue);
    if (d->m_fetchMoreThreshold == fetchMoreThreshold)
        return;

    d->m_fetchMoreThreshold = fetchMoreThreshold;
    emit fetchMoreThresholdChanged(fetchMoreThreshold);
}

/*!
    \qmlproperty enumeration PlayQueue::loadingType
    \brief Holds the currently used loading type used for loading the data.

    \note When changing this property the content will be reset.
*/

/*!
    \property QIfPlayQueue::loadingType
    \brief Holds the currently used loading type used for loading the data.

    \note When changing this property the content will be reset.
*/
QIfPlayQueue::LoadingType QIfPlayQueue::loadingType() const
{
    Q_D(const QIfPlayQueue);
    return d->m_loadingType;
}

void QIfPlayQueue::setLoadingType(QIfPlayQueue::LoadingType loadingType)
{
    Q_D(QIfPlayQueue);
    if (d->m_loadingType == loadingType)
        return;

    if (loadingType == QIfPlayQueue::DataChanged && !d->m_canReportCount) {
        qtif_qmlOrCppWarning(this, "The backend doesn't support the DataChanged loading type. This call will have no effect");
        return;
    }

    d->m_loadingType = loadingType;
    emit loadingTypeChanged(loadingType);

    d->resetModel();
}

/*!
    \qmlproperty int PlayQueue::count
    \brief Holds the current number of rows in this model.
*/
/*!
    \property QIfPlayQueue::count
    \brief Holds the current number of rows in this model.
*/
int QIfPlayQueue::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIfPlayQueue);
    if (parent.isValid())
        return 0;

    return int(d->m_itemList.count());
}

/*!
    \reimp
*/
QVariant QIfPlayQueue::data(const QModelIndex &index, int role) const
{
    Q_D(const QIfPlayQueue);
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row >= d->m_itemList.count() || row < 0)
        return QVariant();

    if (row >= d->m_fetchedDataCount - d->m_fetchMoreThreshold && canFetchMore(QModelIndex()))
        emit fetchMoreThresholdReached();

    const QIfStandardItem *item = d->itemAt(row);
    if (!item)
        return QVariant();

    switch (role) {
        case NameRole: return item->name();
        case TypeRole: return item->type();
        case ItemRole: return d->m_itemList.at(row);
    }

    return QVariant();
}

/*!
    \fn T QIfPlayQueue::at(int i) const

    Returns the item at index \a i converted to the template type T.
*/
/*!
    \qmlmethod object PlayQueue::get(i)

    Returns the item at index \a i.
*/
/*!
    Returns the item at index \a i.

    This function is intended to be used from QML. For C++
    please use the at() instead.
*/
QVariant QIfPlayQueue::get(int i) const
{
    return data(index(i,0), ItemRole);
}

/*!
    \qmlmethod PlayQueue::insert(int index, PlayableItem item)

    Insert the \a item at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.
*/

/*!
    \fn void QIfPlayQueue::insert(int index, const QVariant &variant)

    Insert the \a variant at the position \a index.

    If the backend doesn't accept the provided item, this operation will end in a no op.
*/
void QIfPlayQueue::insert(int index, const QVariant &variant)
{
    const QIfPlayableItem *item = qtif_gadgetFromVariant<QIfPlayableItem>(this, variant);
    if (!item)
        return;

    Q_IF_BACKEND(QIfPlayQueue, d->playerBackend(), "Can't insert items without a connected backend");

    backend->insert(index, variant);
}

/*!
    \qmlmethod PlayQueue::remove(int index)

    Removes the item at position \a index from the play queue.
*/

/*!
    \fn void QIfPlayQueue::remove(int index)

    Removes the item at position \a index from the play queue.
*/
void QIfPlayQueue::remove(int index)
{
    Q_IF_BACKEND(QIfPlayQueue, d->playerBackend(), "Can't remove items without a connected backend");

    backend->remove(index);
}

/*!
    \qmlmethod PlayQueue::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index the play queue.
*/

/*!
    \fn void QIfPlayQueue::move(int cur_index, int new_index)

    Moves the item at position \a cur_index to the new position \a new_index the play queue.
*/
void QIfPlayQueue::move(int cur_index, int new_index)
{
    Q_IF_BACKEND(QIfPlayQueue, d->playerBackend(), "Can't move items without a connected backend");

    backend->move(cur_index, new_index);
}

/*!
    \reimp
*/
bool QIfPlayQueue::canFetchMore(const QModelIndex &parent) const
{
    Q_D(const QIfPlayQueue);
    if (parent.isValid())
        return false;

    return d->m_moreAvailable;
}

/*!
    \reimp
*/
void QIfPlayQueue::fetchMore(const QModelIndex &parent)
{
    Q_D(QIfPlayQueue);
    if (parent.isValid())
        return;

    if (!d->playerBackend())
        return;

    d->m_moreAvailable = false;
    d->playerBackend()->fetchData(d->m_identifier, d->m_fetchedDataCount, d->m_chunkSize);
}

/*!
    \reimp
*/
QHash<int, QByteArray> QIfPlayQueue::roleNames() const
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
    Creates a play queue for the QIfMediaPlayer instance \a parent.
*/
QIfPlayQueue::QIfPlayQueue(QIfMediaPlayer *parent)
    : QAbstractListModel(*new QIfPlayQueuePrivate(parent, this), parent)
{
    Q_D(QIfPlayQueue);
    d->initialize();
}

/*!
    \fn void QIfPlayQueue::fetchMoreThresholdReached() const

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

/*!
    \qmlsignal PlayQueue::fetchMoreThresholdReached()

    This signal is emitted whenever the fetchMoreThreshold is reached and new data is requested from the backend.
*/

QT_END_NAMESPACE

#include "moc_qifplayqueue.cpp"
