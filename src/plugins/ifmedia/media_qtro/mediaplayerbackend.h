// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef MEDIAPLAYERBACKEND_H
#define MEDIAPLAYERBACKEND_H

#include <QtIfMedia/QIfMediaPlayerBackendInterface>
#include <QtIfRemoteObjectsHelper/private/qifremoteobjectsreplicahelper_p.h>
#include <QRemoteObjectNode>
#include "rep_qifmediaplayer_replica.h"

class MediaPlayerBackend : public QIfMediaPlayerBackendInterface
{
public:
    explicit MediaPlayerBackend(QObject *parent = nullptr);

public:
    void initialize() override;
    void play() override;
    void pause() override;
    void stop() override;
    void seek(qint64 offset) override;
    void next() override;
    void previous() override;
    void setPlayMode(QIfMediaPlayer::PlayMode playMode) override;
    void setPosition(qint64 position) override;
    void setCurrentIndex(int currentIndex) override;
    void setVolume(int volume) override;
    void setMuted(bool muted) override;
    void fetchData(const QUuid &identifier, int start, int count) override;
    void insert(int index, const QVariant &item) override;
    void remove(int index) override;
    void move(int currentIndex, int newIndex) override;

    void updateServiceSettings(const QVariantMap &settings);

protected:
    void setupConnections();
    bool connectToNode();

private:
    QSharedPointer<QIfMediaPlayerReplica> m_replica;
    QRemoteObjectNode *m_node;
    QUrl m_url;
    QIfRemoteObjectsReplicaHelper *m_helper;
    QVariantMap m_serviceSettings;
};

#endif // MEDIAPLAYERBACKEND_H
