
/****************************************************************************
** Copyright (C) 2022 The Qt Company Ltd.
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef QIFMEDIAPLAYERQTROADAPTER_H
#define QIFMEDIAPLAYERQTROADAPTER_H

#include <QIfRemoteObjectsSourceHelper>

#include "mediaplayerbackend.h"
#include "rep_qifmediaplayer_source.h"

template <class ObjectType>
struct QIfMediaPlayerAddressWrapper: public QIfMediaPlayerSourceAPI<ObjectType> {
    QIfMediaPlayerAddressWrapper(ObjectType *object)
        : QIfMediaPlayerSourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};

class QIfMediaPlayerQtRoAdapter : public QIfMediaPlayerSource
{
public:
    QIfMediaPlayerQtRoAdapter(MediaPlayerBackend *parent);
    QIfMediaPlayerQtRoAdapter(const QString& remoteObjectsLookupName, MediaPlayerBackend *parent);

    QString remoteObjectsLookupName() const;

public:
    QIfMediaPlayer::PlayMode playMode() const override;
    QIfMediaPlayer::PlayState playState() const override;
    qint64 position() const override;
    qint64 duration() const override;
    QVariant currentTrack() const override;
    int currentIndex() const override;
    int volume() const override;
    bool muted() const override;
    bool canReportCount() const override;
    void setPlayMode(QIfMediaPlayer::PlayMode playMode) override;
    void setPosition(qint64 position) override;
    void setCurrentIndex(int currentIndex) override;
    void setVolume(int volume) override;
    void setMuted(bool muted) override;

public Q_SLOTS:
    void play() override;
    void pause() override;
    void stop() override;
    void seek(qint64 offset) override;
    void next() override;
    void previous() override;
    void fetchData(const QUuid &identifier, int start, int count) override;
    void insert(int index, const QVariant &item) override;
    void remove(int index) override;
    void move(int currentIndex, int newIndex) override;

private:
    QString m_remoteObjectsLookupName;
    MediaPlayerBackend *m_backend;
    QIfRemoteObjectsSourceHelper<QIfMediaPlayerQtRoAdapter> m_helper;
};

#endif // QIFMEDIAPLAYERQTROADAPTER_H
