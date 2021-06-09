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

#ifndef QIFMEDIAPLAYERBACKENDINTERFACE_H
#define QIFMEDIAPLAYERBACKENDINTERFACE_H

#include <QtInterfaceFramework/QIfFeatureInterface>
#include <QtInterfaceFramework/qifqmlconversion_helper.h>
#include <QtIfMedia/qtifmediaglobal.h>
#include <QtIfMedia/QIfMediaPlayer>
#include <QtCore/QUuid>

QT_BEGIN_NAMESPACE

class QIfPlayableItem;

class Q_QTIFMEDIA_EXPORT QIfMediaPlayerBackendInterface : public QIfFeatureInterface
{
    Q_OBJECT

public:
    explicit QIfMediaPlayerBackendInterface(QObject *parent = nullptr);

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void seek(qint64 offset) = 0;
    virtual void next() = 0;
    virtual void previous() = 0;
    virtual void setPlayMode(QIfMediaPlayer::PlayMode playMode) = 0;
    virtual void setPosition(qint64 position) = 0;
    virtual void setCurrentIndex(int currentIndex) = 0;
    virtual void setVolume(int volume) = 0;
    virtual void setMuted(bool muted) = 0;

    virtual void fetchData(const QUuid &identifier, int start, int count) = 0;

    virtual void insert(int index, const QVariant &item) = 0;
    virtual void remove(int index) = 0;
    virtual void move(int currentIndex, int newIndex) = 0;

Q_SIGNALS:
    void playModeChanged(QIfMediaPlayer::PlayMode playMode = QIfMediaPlayer::Normal);
    void playStateChanged(QIfMediaPlayer::PlayState playState  = QIfMediaPlayer::Stopped);
    void currentTrackChanged(const QVariant &currentTrack = QVariant()); //TODO Do we need this or is the currentIndex + the playlistdata enough ?
    void positionChanged(qint64 position = -1);
    //TODO do we need durationChanged, we can get that from the currentTrack metadata.
    void durationChanged(qint64 duration = -1);
    void currentIndexChanged(int currentIndex = -1);
    void volumeChanged(int volume = -1);
    void mutedChanged(bool muted = false);
    void canReportCountChanged(bool canReportCount = false);
    void countChanged(int newLength = -1);
    void dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable);
    void dataChanged(const QList<QVariant> &data, int start, int count);
};

#define QIfMediaPlayer_iid "org.qt-project.interfaceframework.MediaPlayer/1.0"

QT_END_NAMESPACE

#endif // QIFMEDIAPLAYERBACKENDINTERFACE_H
