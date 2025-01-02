// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef MEDIAPLAYERBACKEND_H
#define MEDIAPLAYERBACKEND_H

#include <QtIfMedia/QIfMediaPlayerBackendInterface>

#include <QSqlDatabase>
#include <QtMultimedia/QMediaPlayer>

QT_FORWARD_DECLARE_CLASS(QMediaPlaylist);
QT_FORWARD_DECLARE_CLASS(QThreadPool);

class MediaPlayerBackend : public QIfMediaPlayerBackendInterface
{
    Q_OBJECT
    Q_PROPERTY(QIfMediaPlayer::PlayMode playMode READ playMode WRITE setPlayMode NOTIFY playModeChanged)
    Q_PROPERTY(QIfMediaPlayer::PlayState playState READ playState NOTIFY playStateChanged)
    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
    Q_PROPERTY(QVariant currentTrack READ currentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)

public:
    enum OperationType {
        Select,
        Insert,
        Remove,
        Move,
        SetIndex
    };
    Q_ENUM(OperationType)

    MediaPlayerBackend(const QVariantMap &serviceSettings, const QSqlDatabase &database, QObject *parent = nullptr);

    void initialize() override;
    void play() override;
    void pause() override;
    void stop() override;
    void seek(qint64 offset) override;
    void next() override;
    void previous() override;

    QIfMediaPlayer::PlayMode playMode() const;
    QIfMediaPlayer::PlayState playState() const;
    qint64 position() const;
    qint64 duration() const;
    QVariant currentTrack() const;
    int currentIndex() const;
    int volume() const;
    bool isMuted() const;
    bool canReportCount() const;

signals:
    void playTrack(const QUrl& url);
public Q_SLOTS:
    void setPlayMode(QIfMediaPlayer::PlayMode playMode) override;
    void setPosition(qint64 position) override;
    void setCurrentIndex(int index) override;
    void setVolume(int volume) override;
    void setMuted(bool muted) override;

    void fetchData(const QUuid &identifier, int start, int count) override;

    void insert(int index, const QVariant &i) override;
    void remove(int index) override;
    void move(int cur_index, int new_index) override;

    void doSqlOperation(MediaPlayerBackend::OperationType type, const QStringList &queries, const QUuid &identifier, int start, int count);

private Q_SLOTS:
    void onStateChanged(QMediaPlayer::PlaybackState state);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onPlayTrack(const QUrl& url);
private:

    int m_count;
    int m_currentIndex;
    QVariant m_currentTrack;
    QIfMediaPlayer::PlayMode m_playMode;
    QIfMediaPlayer::PlayState m_requestedState;
    QIfMediaPlayer::PlayState m_state;
    QThreadPool *m_threadPool;
    QMediaPlayer *m_player;
    QSqlDatabase m_db;
};

#endif // MEDIAPLAYERBACKEND_H
