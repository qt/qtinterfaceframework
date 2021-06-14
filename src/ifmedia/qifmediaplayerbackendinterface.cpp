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

#include "qifmediaplayerbackendinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIfMediaPlayerBackendInterface
    \inmodule QtIfMedia
    \ingroup backends
    \inherits QObject
    \keyword org.qt-project.interfaceframework.MediaPlayer/1.0
    \brief The QIfMediaPlayerBackendInterface defines the interface for backends to the
    QIfMediaPlayer feature class.

    The QIfMediaPlayerBackendInterface is the interface used by \l QIfMediaPlayer

    The interface is discovered by a \l QIfMediaPlayer object, which connects to it and sets it up.

    <example of a fully featured backend>
*/

/*!
    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIfMediaPlayerBackendInterface::QIfMediaPlayerBackendInterface(QObject *parent)
    : QIfFeatureInterface(parent)
{

}

/*!
    \fn QIfMediaPlayerBackendInterface::play()

    Starts playing the current playable item.

    \sa pause() stop()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::pause()

    Pauses the playback of the current playable item.

    \sa play() stop()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::stop()

    Stops playing the current playable item.

    \sa play() stop()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::seek(qint64 offset)

    Seeks the current playable item using \a offset in ms.

    The offset can be positive or negative to either seek forward
    or backward. A successful seek will result in a change of the
    position property.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::next()

    Skips to the next playable item.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::previous()

    Skips to the previous playable item.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::setPlayMode(QIfMediaPlayer::PlayMode playMode)

    Sets \a playMode as the new playback mode for the player. If a valid playMode is passed, the
    function should emit the corresponding change signal.

    \sa playModeChanged
*/

/*!
    \fn QIfMediaPlayerBackendInterface::setPosition(qint64 position)

    Sets the \a position of the currently playing item. If a valid position is passed, the
    function should emit the corresponding change signals

    \sa positionChanged
*/

/*!
    \fn QIfMediaPlayerBackendInterface::setCurrentIndex(int currentIndex)

    Sets the \a currentIndex of the play-queue to the given index. If a valid index is passed, the
    function should update the current playable item and emit the corresponding change signals

    \sa currentIndexChanged currentTrackChanged durationChanged
*/

/*!
    \fn QIfMediaPlayerBackendInterface::setVolume(int volume)

    Sets the current player \a volume. The value needs to be between 0 and 100.

    \sa volumeChanged
*/

/*!
    \fn QIfMediaPlayerBackendInterface::setMuted(bool muted)

    When \a muted is set to \e true, the player will be muted and un-muted otherwise.

    \sa mutedChanged
*/

/*!
    \fn QIfMediaPlayerBackendInterface::fetchData(const QUuid &identifier, int start, int count)

    This function is called whenever new playable items needs to be retrieved by the QIfPlayQueue.

    The parameters \a start and \a count define the range of data to be fetched. This method is expected to emit the dataFetched() signal when
    the new data is ready. Use \a identifier to identify the calling QIfPlayQueue instance.

    \sa dataFetched()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::insert(int index, const QVariant &item)

    Adds the playable item identitifed by \a item into the play queue at \a index.

    \sa dataChanged()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::remove(int index)

    Removes the playable item at position \a index from the play queue.

    \sa dataChanged()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::move(int currentIndex, int newIndex)

    Moves the playable item at position \a currentIndex of the play queue to the new position \a newIndex.

    \sa dataChanged()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::playModeChanged(QIfMediaPlayer::PlayMode playMode);

    Emitted when the play mode changed. The new play mode will be passed as \a playMode.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::playStateChanged(QIfMediaPlayer::PlayState playState);

    Emitted when the play state changed. The new play state will be passed as \a playState.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::currentTrackChanged(const QVariant &currentTrack)

    Emitted when the currently played playable item changed. The new playable item will be passed as \a currentTrack.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::positionChanged(qint64 position)

    Emitted when the position of the currently playing playable item changed. The new position will be passed as \a position in ms.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::durationChanged(qint64 duration)

    Emitted when the duration of the currently playing playable item changed. The new duration will be passed as \a duration in ms.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::currentIndexChanged(int currentIndex)

    Emitted when the currently played playable item in the play queue changed. The index of the new current playable item will be passed as \a currentIndex.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::volumeChanged(int volume)

    Emitted when the volume of this player changed. The new volume of the player will be passed as \a volume.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::mutedChanged(bool muted)

    Emitted when the player changes its mute state. The player is muted when \a muted is set to \e true.
*/

/*!
    \fn QIfMediaPlayerBackendInterface::canReportCountChanged(bool canReportCount)

    This signal is emitted if the backend can return the final number of items (\a canReportCount) for a specific request. This makes it possible to support the QIfPlayQueue::DataChanged loading
    type. The number of items can then be returned by emitting the countChanged signal.

    \sa countChanged()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::countChanged(int newLength)

    This signal is emitted once the backend knows about the new size of the play queue.
    The new number of items is returned as \a newLength.

    This signal is expected to be emitted before the data is returned by emitting the dataFetched() signal.

    \sa fetchData() dataFetched()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable)

    This signal is emitted as a result of a call to fetchData() and returns the requested \a data to the QIfPlayQueue instance identified by \a identifier.
    The arguments \a start holds the index where the data starts, \a moreAvailable holds whether there is more data available.
    To retrieve this data, use a new fetchData() call.

    \sa fetchData() dataFetched()
*/

/*!
    \fn QIfMediaPlayerBackendInterface::dataChanged(const QList<QVariant> &data, int start, int count)

    This signal is emitted whenever the play queue changes, e.g. by a insert, remove or a move operation.
    The \a data argument holds the new data which will replace the data starting at \a start until \a count.

    If \a data is empty the rows identified by the range of \a start and \a count will be removed.
    If \a count is 0, \a data will be inserted at \a start.

    For inserting a new item, the item is passed in \a data and \a start is used for where the item should be inserted, the \a count argument needs to be 0 as we don't want to replace existing data:

    \code
    QList<ExampleItem> list;
    ExampleItem item = ExampleItem();
    list.insert(index, item);
    QVariantList items = { QVariant::fromValue(item) };
    emit dataChanged(items, index, 0);
    \endcode

    Removing an item is very similar, \a start is used to indicate which item and \a count to indicate how much:

    \code
    list.removeAt(index);
    emit dataChanged(QVariantList(), index, 1);
    \endcode

    \sa insert() remove() move()
*/

QT_END_NAMESPACE
