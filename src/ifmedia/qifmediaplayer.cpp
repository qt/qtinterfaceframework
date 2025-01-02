// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifmediaplayer.h"
#include "qifmediaplayer_p.h"
#include "qifplayqueue.h"
#include "qifplayqueue_p.h"
#include "qifqmlconversion_helper.h"
#include <QtInterfaceFramework/QIfServiceObject>
#include <QtDebug>
#include <QDataStream>
#include <QMetaEnum>

QT_BEGIN_NAMESPACE

QIfMediaPlayerPrivate::QIfMediaPlayerPrivate(const QString &interface, QIfMediaPlayer *parent)
    : QIfAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_playQueue(nullptr)
    , m_playMode(QIfMediaPlayer::Normal)
    , m_playState(QIfMediaPlayer::Stopped)
    , m_currentTrack(nullptr)
    , m_position(-1)
    , m_duration(-1)
    , m_volume(0)
    , m_muted(false)
{
    qRegisterMetaType<QIfPlayQueue*>();
    qRegisterMetaType<QIfPlayableItem>();
    qRegisterMetaType<QIfAudioTrackItem>();
}

void QIfMediaPlayerPrivate::initialize()
{
    QIfAbstractFeaturePrivate::initialize();
    m_playQueue = new QIfPlayQueue(q_ptr);
}

void QIfMediaPlayerPrivate::clearToDefaults()
{
    onPlayModeChanged(QIfMediaPlayer::Normal);
    onCurrentTrackChanged(QVariant());
    onPositionChanged(-1);
    onDurationChanged(-1);
    onVolumeChanged(0);
    onMutedChanged(false);
    m_playQueue->d_func()->clearToDefaults();
}

void QIfMediaPlayerPrivate::onPlayModeChanged(QIfMediaPlayer::PlayMode playMode)
{
    if (m_playMode == playMode)
        return;

    Q_Q(QIfMediaPlayer);
    m_playMode = playMode;
    emit q->playModeChanged(playMode);
}

void QIfMediaPlayerPrivate::onPlayStateChanged(QIfMediaPlayer::PlayState playState)
{
    if (m_playState == playState)
        return;

    Q_Q(QIfMediaPlayer);
    m_playState = playState;
    emit q->playStateChanged(playState);
}

void QIfMediaPlayerPrivate::onCurrentTrackChanged(const QVariant &currentTrack)
{
    if (m_currentTrackData == currentTrack)
        return;

    const QIfPlayableItem *currentItem = nullptr;

    if (currentTrack.isValid()) {
        currentItem = qtif_gadgetFromVariant<QIfPlayableItem>(q_ptr, currentTrack);
        if (m_currentTrack == currentItem)
            return;
    }

    Q_Q(QIfMediaPlayer);
    m_currentTrackData = currentTrack;
    m_currentTrack = currentItem;
    emit q->currentTrackChanged(m_currentTrackData);
}

void QIfMediaPlayerPrivate::onPositionChanged(qint64 position)
{
    if (m_position == position)
        return;
    Q_Q(QIfMediaPlayer);
    m_position = position;
    emit q->positionChanged(position);
}

void QIfMediaPlayerPrivate::onDurationChanged(qint64 duration)
{
    if (m_duration == duration)
        return;
    Q_Q(QIfMediaPlayer);
    m_duration = duration;
    emit q->durationChanged(duration);
}

void QIfMediaPlayerPrivate::onVolumeChanged(int volume)
{
    if (m_volume == volume)
        return;
    Q_Q(QIfMediaPlayer);
    m_volume = volume;
    emit q->volumeChanged(volume);
}

void QIfMediaPlayerPrivate::onMutedChanged(bool muted)
{
    if (m_muted == muted)
        return;
    Q_Q(QIfMediaPlayer);
    m_muted = muted;
    emit q->mutedChanged(muted);
}

QIfMediaPlayerBackendInterface *QIfMediaPlayerPrivate::playerBackend() const
{
    return backend<QIfMediaPlayerBackendInterface*>();
}

/*!
    \class QIfMediaPlayer
    \inmodule QtIfMedia
    \brief Provides an interface to control a media player.

    The QIfMediaPlayer provides methods to control a media player. This media player can
    be local or even a remote device you are connected to e.g. over bluetooth.

    By default the autoDiscovery is turned to Automatic for this feature and most likely will connect to
    a local media player instance.

    Uses \c qtifmedia as configurationId for \l QIfConfiguration based settings.
*/

/*!
    \qmltype MediaPlayer
    \instantiates QIfMediaPlayer
    \inqmlmodule QtInterfaceFramework.Media
    \inherits AbstractFeature
    \brief Provides an interface to control a media player.

    The MediaPlayer provides methods to control a media player. This media player can
    be local or even a remote device you are connected to e.g. over bluetooth.

    By default the autoDiscovery is turned to Automatic for this feature and most likely will connect to
    a local media player instance.

    Uses \c qtifmedia as configurationId for \l InterfaceFrameworkConfiguration based settings.
*/

/*!
    \enum QIfMediaPlayer::PlayMode
    \value Normal
          Each item in the queue is played in sequential order. Usually the playback stops when the end
          of the queue is reached.
    \value RepeatTrack
          Always repeat the current item. It should still be possible to change the current item
          using next() and previous(), but this depends on the implementation of the backend.
    \value RepeatAll
          When the end of the queue is reached, the first item starts to play.
    \value Shuffle
          The item in the queue are played in an random order.
*/

/*!
    \enum QIfMediaPlayer::PlayState
    \value Playing
          The media player is currently playing an item.
    \value Paused
          The playback is paused and can be continued at the same position.
    \value Stopped
          The playback hasn't been started yet. Starting it, will always start from the beginning.
*/

/*!
    Constructs a QIfMediaPlayer.

    The \a parent argument is passed on to the \l QIfAbstractFeature base class.
*/
QIfMediaPlayer::QIfMediaPlayer(QObject *parent)
    : QIfAbstractFeature(*new QIfMediaPlayerPrivate(QLatin1String(QIfMediaPlayer_iid), this), parent)
{
    setConfigurationId(QStringLiteral("qtifmedia"));
}

/*!
    \qmlproperty PlayQueue MediaPlayer::playQueue
    \brief Holds the play queue of this media player.

    \sa PlayQueue
*/
/*!
    \property QIfMediaPlayer::playQueue
    \brief Holds the play queue of this media player.

    \sa QIfPlayQueue
*/
QIfPlayQueue *QIfMediaPlayer::playQueue() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_playQueue;
}

/*!
    \qmlproperty enumeration MediaPlayer::playMode
    \brief Holds the current playback mode of the media player.
    Available values are:
    \value Normal
           Each item in the queue is played in sequential order. Usually the playback stops when the end
           of the queue is reached.
    \value RepeatTrack
           Always repeat the current item. It should still be possible to change the current item
           using next() and previous(), but this depends on the implementation of the backend.
    \value RepeatAll
           When the end of the queue is reached, the first item starts to play.
    \value Shuffle
           The item in the queue are played in an random order.
*/
/*!
    \property QIfMediaPlayer::playMode
    \brief Holds the current playback mode of the media player.
*/
QIfMediaPlayer::PlayMode QIfMediaPlayer::playMode() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_playMode;
}

/*!
    \qmlproperty enumeration MediaPlayer::playState
    \brief Holds the current playback state of the media player.
    Available values are:
    \value Playing
           The media player is currently playing an item.
    \value Paused
           The playback is paused and can be continued at the same position.
    \value Stopped
           The playback hasn't been started yet. Starting it, will always start from the beginning.
*/
/*!
    \property QIfMediaPlayer::playState
    \brief Holds the current playback state of the media player.
*/
QIfMediaPlayer::PlayState QIfMediaPlayer::playState() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_playState;
}

/*!
    \qmlproperty object MediaPlayer::currentTrack
    \brief Holds the current track represented as QVariant.

    \note This will be replaced by soon.
*/
/*!
    \property QIfMediaPlayer::currentTrack
    \brief Holds the current track represented as QVariant.

    \note This will be replaced by soon.
*/
QVariant QIfMediaPlayer::currentTrack() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_currentTrackData;
}

/*!
    \qmlproperty int MediaPlayer::position
    \brief Holds the position of the current song of the media player in seconds.
*/
/*!
    \property QIfMediaPlayer::position
    \brief Holds the position of the current song of the media player in seconds.
*/
qint64 QIfMediaPlayer::position() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_position;
}

/*!
    \qmlproperty int MediaPlayer::duration
    \brief Holds the total duration of the current song in seconds.
*/
/*!
    \property QIfMediaPlayer::duration
    \brief Holds the total duration of the current song in seconds.
*/
qint64 QIfMediaPlayer::duration() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_duration;
}

/*!
    \qmlproperty int MediaPlayer::volume
    \brief Holds the sound volume level (0..100)
    \sa muted
*/
/*!
    \property QIfMediaPlayer::volume
    \brief Holds the sound volume level (0..100)
    \sa muted
*/
int QIfMediaPlayer::volume() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_volume;
}


/*!
    \qmlproperty bool MediaPlayer::muted
    \brief This property holds whether the audio output is muted.
    \sa volume
*/
/*!
    \property QIfMediaPlayer::muted
    \brief This property holds whether the audio output is muted.
    \sa volume
*/
bool QIfMediaPlayer::isMuted() const
{
    Q_D(const QIfMediaPlayer);
    return d->m_muted;
}

void QIfMediaPlayer::setPlayMode(QIfMediaPlayer::PlayMode playMode)
{
    Q_D(QIfMediaPlayer);
    QIfMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't set the play mode without a connected backend");
        return;
    }

    backend->setPlayMode(playMode);
}

void QIfMediaPlayer::setPosition(qint64 position)
{
    Q_D(QIfMediaPlayer);
    QIfMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't set the position without a connected backend");
        return;
    }

    backend->setPosition(position);
}

/*!
    \qmlmethod MediaPlayer::play()

    Starts to play the current track. If the playQueue is empty
    it's up to the backend to decide what to do.

    \sa pause() stop()
*/

/*!
    \fn void QIfMediaPlayer::play()

    Starts to play the current track. If the playQueue is empty
    it's up to the backend to decide what to do.

    \sa pause() stop()
*/
void QIfMediaPlayer::play()
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't start playing without a connected backend");

    backend->play();
}

/*!
    \qmlmethod MediaPlayer::pause()

    Pauses the currently ongoing playback.

    \sa play() stop()
*/

/*!
    \fn void QIfMediaPlayer::pause()

    Pauses the currently ongoing playback.

    \sa play() stop()
*/
void QIfMediaPlayer::pause()
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't pause playing without a connected backend");

    backend->pause();
}

/*!
    \qmlmethod MediaPlayer::stop()

    Stops the currently ongoing playback.

    \sa play() pause()
*/

/*!
    \fn void QIfMediaPlayer::stop()

    Stops the currently ongoing playback.

    \sa play() pause()
*/
void QIfMediaPlayer::stop()
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't stop playing without a connected backend");

    backend->stop();
}

/*!
    \qmlmethod MediaPlayer::seek(offset)

    Seeks into the current track using \a offset.

    The offset can be positive or negative to either seek forward
    or backward. A successful seek will result in a change of the
    position property.
*/

/*!
    \fn void QIfMediaPlayer::seek(qint64 offset)

    Seeks into the current track using \a offset.

    The offset can be positive or negative to either seek forward
    or backward. A successful seek will result in a change of the
    position property.
*/
void QIfMediaPlayer::seek(qint64 offset)
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't seek without a connected backend");

    backend->seek(offset);
}

/*!
    \qmlmethod MediaPlayer::next()

    Skips to the next track in the playQueue.

    \sa playMode
*/

/*!
    \fn void QIfMediaPlayer::next()

    Skips to the next track in the playQueue.

    \sa playMode
*/
void QIfMediaPlayer::next()
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't skip to the next track without a connected backend");

    backend->next();
}

/*!
    \qmlmethod MediaPlayer::previous()

    Skips to the previous track in the playQueue.

    \sa playMode
*/

/*!
    \fn void QIfMediaPlayer::previous()

    Skips to the previous track in the playQueue.

    \sa playMode
*/
void QIfMediaPlayer::previous()
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't skip to the previous track without a connected backend");

    backend->previous();
}

void QIfMediaPlayer::setVolume(int volume)
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't set the volume without a connected backend");

    backend->setVolume(volume);
}

void QIfMediaPlayer::setMuted(bool muted)
{
    Q_IF_BACKEND(QIfMediaPlayer, d->playerBackend(), "Can't set muted without a connected backend");

    backend->setMuted(muted);
}

/*!
    \internal
*/
QIfMediaPlayer::QIfMediaPlayer(QIfMediaPlayerPrivate &dd, QObject *parent)
    : QIfAbstractFeature(dd, parent)
{
}

/*!
    \reimp
*/
void QIfMediaPlayer::connectToServiceObject(QIfServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIfMediaPlayer);

    QIfMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::playModeChanged,
                            d, &QIfMediaPlayerPrivate::onPlayModeChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::playStateChanged,
                            d, &QIfMediaPlayerPrivate::onPlayStateChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::positionChanged,
                            d, &QIfMediaPlayerPrivate::onPositionChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::currentTrackChanged,
                            d, &QIfMediaPlayerPrivate::onCurrentTrackChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::durationChanged,
                            d, &QIfMediaPlayerPrivate::onDurationChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::volumeChanged,
                            d, &QIfMediaPlayerPrivate::onVolumeChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::mutedChanged,
                            d, &QIfMediaPlayerPrivate::onMutedChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::canReportCountChanged,
                            d->m_playQueue->d_func(), &QIfPlayQueuePrivate::onCanReportCountChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::dataFetched,
                            d->m_playQueue->d_func(), &QIfPlayQueuePrivate::onDataFetched);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::countChanged,
                            d->m_playQueue->d_func(), &QIfPlayQueuePrivate::onCountChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::dataChanged,
                            d->m_playQueue->d_func(), &QIfPlayQueuePrivate::onDataChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::currentIndexChanged,
                            d->m_playQueue->d_func(), &QIfPlayQueuePrivate::onCurrentIndexChanged);
    QObjectPrivate::connect(backend, &QIfMediaPlayerBackendInterface::initializationDone,
                            d->m_playQueue->d_func(), &QIfPlayQueuePrivate::onInitializationDone);

    QIfAbstractFeature::connectToServiceObject(serviceObject);
}

/*!
    \reimp
*/
void QIfMediaPlayer::clearServiceObject()
{
    Q_D(QIfMediaPlayer);
    d->clearToDefaults();
}

QDataStream &operator <<(QDataStream &out, QIfMediaPlayer::PlayMode var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIfMediaPlayer::PlayMode &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIfMediaPlayer::PlayMode>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIfMediaPlayer::PlayMode, value =" << val;
    var = QIfMediaPlayer::PlayMode(val);
    return in;
}

QDataStream &operator <<(QDataStream &out, QIfMediaPlayer::PlayState var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIfMediaPlayer::PlayState &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIfMediaPlayer::PlayState>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIfMediaPlayer::PlayState, value =" << val;
    var = QIfMediaPlayer::PlayState(val);
    return in;
}

QT_END_NAMESPACE

#include "moc_qifmediaplayer.cpp"
