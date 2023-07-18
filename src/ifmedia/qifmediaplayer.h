// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
    QML_NAMED_ELEMENT(MediaPlayer)

    //TODO Status
    Q_PROPERTY(QIfPlayQueue *playQueue READ playQueue CONSTANT FINAL)
    Q_PROPERTY(QIfMediaPlayer::PlayMode playMode READ playMode WRITE setPlayMode NOTIFY playModeChanged FINAL)
    Q_PROPERTY(QIfMediaPlayer::PlayState playState READ playState NOTIFY playStateChanged FINAL)
    Q_PROPERTY(QVariant currentTrack READ currentTrack NOTIFY currentTrackChanged FINAL)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged FINAL)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged FINAL)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged FINAL)

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
