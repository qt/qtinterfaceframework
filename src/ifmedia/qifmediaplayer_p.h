// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFMEDIAPLAYER_P_H
#define QIFMEDIAPLAYER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qtifmediaglobal_p.h"
#include <QtInterfaceFramework/private/qifabstractfeature_p.h>

#include "qifmediaplayer.h"
#include "qifmediaplayerbackendinterface.h"

QT_BEGIN_NAMESPACE

class QIfMediaPlayerPrivate : public QIfAbstractFeaturePrivate
{
public:
    QIfMediaPlayerPrivate(const QString &interface, QIfMediaPlayer *parent);

    void initialize() override;
    void clearToDefaults();
    void onPlayModeChanged(QIfMediaPlayer::PlayMode playMode);
    void onPlayStateChanged(QIfMediaPlayer::PlayState playState);
    void onCurrentTrackChanged(const QVariant &currentTrack);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onVolumeChanged(int volume);
    void onMutedChanged(bool muted);

    QIfMediaPlayerBackendInterface *playerBackend() const;

    QIfMediaPlayer * const q_ptr;
    QIfPlayQueue *m_playQueue;
    QIfMediaPlayer::PlayMode m_playMode;
    QIfMediaPlayer::PlayState m_playState;
    QVariant m_currentTrackData;
    const QIfPlayableItem *m_currentTrack;
    qint64 m_position;
    qint64 m_duration;
    int m_volume;
    bool m_muted;
    Q_DECLARE_PUBLIC(QIfMediaPlayer)
};

QT_END_NAMESPACE

#endif // QIFMEDIAPLAYER_P_H
