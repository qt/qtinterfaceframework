/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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

#include "qifmediaplayerqtroadapter.h"
#include "qifqmlconversion_helper.h"

QIfMediaPlayerQtRoAdapter::QIfMediaPlayerQtRoAdapter(MediaPlayerBackend *parent)
    : QIfMediaPlayerQtRoAdapter(QStringLiteral("QtIfMedia.QIfMediaPlayer"), parent)
{
}

QIfMediaPlayerQtRoAdapter::QIfMediaPlayerQtRoAdapter(const QString& remoteObjectsLookupName, MediaPlayerBackend *parent)
    : QIfMediaPlayerSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
    , m_helper(this)
{
    connect(m_backend, &MediaPlayerBackend::playModeChanged, this, &QIfMediaPlayerQtRoAdapter::playModeChanged);
    connect(m_backend, &MediaPlayerBackend::playStateChanged, this, &QIfMediaPlayerQtRoAdapter::playStateChanged);
    connect(m_backend, &MediaPlayerBackend::positionChanged, this, &QIfMediaPlayerQtRoAdapter::positionChanged);
    connect(m_backend, &MediaPlayerBackend::durationChanged, this, &QIfMediaPlayerQtRoAdapter::durationChanged);
    connect(m_backend, &MediaPlayerBackend::currentIndexChanged, this, &QIfMediaPlayerQtRoAdapter::currentIndexChanged);
    connect(m_backend, &MediaPlayerBackend::currentTrackChanged, this, [this](const QVariant &currentTrack) {
        emit currentTrackChanged(QVariant(QMetaType(QMetaType::QVariant), &currentTrack));
    });
    connect(m_backend, &MediaPlayerBackend::volumeChanged, this, &QIfMediaPlayerQtRoAdapter::volumeChanged);
    connect(m_backend, &MediaPlayerBackend::mutedChanged, this, &QIfMediaPlayerQtRoAdapter::mutedChanged);
    connect(m_backend, &MediaPlayerBackend::dataFetched, this, &QIfMediaPlayerQtRoAdapter::dataFetched);
    connect(m_backend, &MediaPlayerBackend::dataChanged, this, &QIfMediaPlayerQtRoAdapter::dataChanged);
    connect(m_backend, &MediaPlayerBackend::countChanged, this, &QIfMediaPlayerQtRoAdapter::countChanged);
}


QString QIfMediaPlayerQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

QIfMediaPlayer::PlayMode QIfMediaPlayerQtRoAdapter::playMode() const
{
    return m_backend->playMode();
}

QIfMediaPlayer::PlayState QIfMediaPlayerQtRoAdapter::playState() const
{
    return m_backend->playState();
}

qint64 QIfMediaPlayerQtRoAdapter::position() const
{
    return m_backend->position();
}

qint64 QIfMediaPlayerQtRoAdapter::duration() const
{
    return m_backend->duration();
}

QVariant QIfMediaPlayerQtRoAdapter::currentTrack() const
{
    return m_helper.toRemoteObjectVariant(m_backend->currentTrack());
}

int QIfMediaPlayerQtRoAdapter::currentIndex() const
{
    return m_backend->currentIndex();
}

int QIfMediaPlayerQtRoAdapter::volume() const
{
    return m_backend->volume();
}

bool QIfMediaPlayerQtRoAdapter::muted() const
{
    return m_backend->isMuted();
}

bool QIfMediaPlayerQtRoAdapter::canReportCount() const
{
    return m_backend->canReportCount();
}

void QIfMediaPlayerQtRoAdapter::setPlayMode(QIfMediaPlayer::PlayMode playMode)
{
    m_backend->setPlayMode(playMode);
}

void QIfMediaPlayerQtRoAdapter::setPosition(qint64 position)
{
    m_backend->setPosition(position);
}

void QIfMediaPlayerQtRoAdapter::setCurrentIndex(int currentIndex)
{
    m_backend->setCurrentIndex(currentIndex);
}

void QIfMediaPlayerQtRoAdapter::setVolume(int volume)
{
    m_backend->setVolume(volume);
}

void QIfMediaPlayerQtRoAdapter::setMuted(bool muted)
{
    m_backend->setMuted(muted);
}

void QIfMediaPlayerQtRoAdapter::play()
{
    m_backend->play();
}

void QIfMediaPlayerQtRoAdapter::pause()
{
    m_backend->pause();
}

void QIfMediaPlayerQtRoAdapter::stop()
{
    m_backend->stop();
}

void QIfMediaPlayerQtRoAdapter::seek(qint64 offset)
{
    m_backend->seek(offset);
}

void QIfMediaPlayerQtRoAdapter::next()
{
    m_backend->next();
}

void QIfMediaPlayerQtRoAdapter::previous()
{
    m_backend->previous();
}

void QIfMediaPlayerQtRoAdapter::fetchData(const QUuid &identifier, int start, int count)
{
    m_backend->fetchData(identifier, start, count);
}

void QIfMediaPlayerQtRoAdapter::insert(int index, const QVariant &item)
{
    m_backend->insert(index, item);
}

void QIfMediaPlayerQtRoAdapter::remove(int index)
{
    m_backend->remove(index);
}

void QIfMediaPlayerQtRoAdapter::move(int currentIndex, int newIndex)
{
    m_backend->move(currentIndex, newIndex);
}
