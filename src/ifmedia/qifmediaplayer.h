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

#ifndef QIFMEDIAPLAYER_H
#define QIFMEDIAPLAYER_H

#include <QtInterfaceFramework/QIfAbstractFeature>
#include <QtIfMedia/QIfPlayableItem>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class QIfMediaPlayerPrivate;
class QIfPlayQueue;

class Q_QTIFMEDIA_EXPORT QIfMediaPlayer : public QIfAbstractFeature
{
    Q_OBJECT

    //TODO Status
    Q_PROPERTY(QIfPlayQueue *playQueue READ playQueue CONSTANT)
    Q_PROPERTY(QIfMediaPlayer::PlayMode playMode READ playMode WRITE setPlayMode NOTIFY playModeChanged)
    Q_PROPERTY(QIfMediaPlayer::PlayState playState READ playState NOTIFY playStateChanged)
    Q_PROPERTY(QVariant currentTrack READ currentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)

public:
    enum PlayMode {
        Normal,
        RepeatTrack,
        RepeatAll,
        Shuffle
    };
    Q_ENUM(PlayMode)

    enum PlayState {
        Playing,
        Paused,
        Stopped
    };
    Q_ENUM(PlayState)

    explicit QIfMediaPlayer(QObject *parent = nullptr);

    QIfPlayQueue *playQueue() const;
    PlayMode playMode() const;
    PlayState playState() const;
    QVariant currentTrack() const;
    qint64 position() const;
    qint64 duration() const;
    int volume() const;
    bool isMuted() const;

public Q_SLOTS:
    void setPlayMode(QIfMediaPlayer::PlayMode playMode);
    void setPosition(qint64 position);
    void play();
    void pause();
    void stop();
    void seek(qint64 offset);
    void next();
    void previous();
    void setVolume(int volume);
    void setMuted(bool muted);

Q_SIGNALS:
    void playModeChanged(QIfMediaPlayer::PlayMode playMode);
    void playStateChanged(QIfMediaPlayer::PlayState playState);
    void currentTrackChanged(const QVariant &currentTrack);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumeChanged(int volume);
    void mutedChanged(bool muted);

protected:
    QIfMediaPlayer(QIfMediaPlayerPrivate &dd, QObject *parent = nullptr);

    void connectToServiceObject(QIfServiceObject *serviceObject) override;
    void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIfMediaPlayer)
    Q_PRIVATE_SLOT(d_func(), void onPlayModeChanged(QIfMediaPlayer::PlayMode playMode))
    Q_PRIVATE_SLOT(d_func(), void onPlayStateChanged(QIfMediaPlayer::PlayState playState))
    Q_PRIVATE_SLOT(d_func(), void onCurrentTrackChanged(const QVariant &currentTrack))
    Q_PRIVATE_SLOT(d_func(), void onPositionChanged(qint64 position))
    Q_PRIVATE_SLOT(d_func(), void onDurationChanged(qint64 duration))
    Q_PRIVATE_SLOT(d_func(), void onVolumeChanged(int volume))
    Q_PRIVATE_SLOT(d_func(), void onMutedChanged(bool muted))

    friend class QIfPlayQueuePrivate;
};

Q_QTIFMEDIA_EXPORT QDataStream &operator<<(QDataStream &out, QIfMediaPlayer::PlayMode var);
Q_QTIFMEDIA_EXPORT QDataStream &operator>>(QDataStream &in, QIfMediaPlayer::PlayMode &var);
Q_QTIFMEDIA_EXPORT QDataStream &operator<<(QDataStream &out, QIfMediaPlayer::PlayState var);
Q_QTIFMEDIA_EXPORT QDataStream &operator>>(QDataStream &in, QIfMediaPlayer::PlayState &var);

QT_END_NAMESPACE

#endif // QIFMEDIAPLAYER_H
