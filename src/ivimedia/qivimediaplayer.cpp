/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qivimediaplayer.h"
#include "qivimediaplayer_p.h"
#include "qiviplayqueue.h"
#include "qiviplayqueue_p.h"
#include <QtIviCore/QIviServiceObject>
#include <QtDebug>

QIviMediaPlayerPrivate::QIviMediaPlayerPrivate(const QString &interface, QIviMediaPlayer *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_playQueue(nullptr)
    , m_currentTrack(0)
    , m_position(-1)
    , m_duration(-1)
{
    qRegisterMetaType<QIviPlayQueue*>();
    qRegisterMetaType<QIviPlayableItem>();
    qRegisterMetaType<QIviAudioTrackItem>();
}

void QIviMediaPlayerPrivate::init()
{
    m_playQueue = new QIviPlayQueue(q_ptr);
}

void QIviMediaPlayerPrivate::clearToDefaults()
{
    m_currentTrackData = QVariant();
    m_currentTrack = 0;
    m_position = -1;
    m_duration = -1;
    m_playQueue->d_func()->clearToDefaults();
}

void QIviMediaPlayerPrivate::onCurrentTrackChanged(const QVariant &currentTrack)
{
    if (m_currentTrackData == currentTrack)
        return;

    const QIviPlayableItem *currentItem = playableItem(m_currentTrackData);
    if (m_currentTrack == currentItem)
        return;

    Q_Q(QIviMediaPlayer);
    m_currentTrackData = currentTrack;
    m_currentTrack = currentItem;
    emit q->currentTrackChanged(m_currentTrackData);
}

void QIviMediaPlayerPrivate::onPositionChanged(int position)
{
    if (m_position == position)
        return;
    Q_Q(QIviMediaPlayer);
    m_position = position;
    emit q->positionChanged(position);
}

void QIviMediaPlayerPrivate::onDurationChanged(int duration)
{
    if (m_duration == duration)
        return;
    Q_Q(QIviMediaPlayer);
    m_duration = duration;
    emit q->durationChanged(duration);
}

//TODO replace me by a generic function for all classes
const QIviPlayableItem *QIviMediaPlayerPrivate::playableItem(const QVariant &item)
{
    const void *data = item.constData();

    QMetaType type(item.userType());
    if (!type.flags().testFlag(QMetaType::IsGadget)) {
        qCritical() << "The passed QVariant needs to use the Q_GADGET macro";
        return nullptr;
    }

    const QMetaObject *mo = type.metaObject();
    while (mo) {
        if (mo->className() == QIviPlayableItem::staticMetaObject.className())
            return reinterpret_cast<const QIviPlayableItem*>(data);
        mo = mo->superClass();
    }

    qCritical() << "The passed QVariant is not derived from QIviPlayableItem";
    return nullptr;
}

QIviMediaPlayerBackendInterface *QIviMediaPlayerPrivate::playerBackend() const
{
    Q_Q(const QIviMediaPlayer);
    QIviServiceObject *so = q->serviceObject();
    if (so)
        return qobject_cast<QIviMediaPlayerBackendInterface*>(so->interfaceInstance(QIviStringMediaPlayerInterfaceName));

    return nullptr;
}

QIviMediaPlayer::QIviMediaPlayer(QObject *parent)
    : QIviAbstractFeature(*new QIviMediaPlayerPrivate(QIviStringMediaPlayerInterfaceName, this), parent)
{
    Q_D(QIviMediaPlayer);
    d->init();
}

QIviPlayQueue *QIviMediaPlayer::playQueue() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_playQueue;
}

QVariant QIviMediaPlayer::currentTrack() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_currentTrackData;
}

int QIviMediaPlayer::position() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_position;
}

int QIviMediaPlayer::duration() const
{
    Q_D(const QIviMediaPlayer);
    return d->m_duration;
}

void QIviMediaPlayer::play()
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't start playing without a connected backend");
        return;
    }

    backend->play();
}

void QIviMediaPlayer::pause()
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't pause playing without a connected backend");
        return;
    }

    backend->pause();
}

void QIviMediaPlayer::stop()
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't stop playing without a connected backend");
        return;
    }

    backend->stop();
}

void QIviMediaPlayer::seek(int offset)
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't seek without a connected backend");
        return;
    }

    backend->seek(offset);
}

void QIviMediaPlayer::next()
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't skip to the next track without a connected backend");
        return;
    }

    backend->next();
}

void QIviMediaPlayer::previous()
{
    Q_D(QIviMediaPlayer);
    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend) {
        qWarning("Can't skip to the next track without a connected backend");
        return;
    }

    backend->previous();
}

QIviMediaPlayer::QIviMediaPlayer(QIviMediaPlayerPrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
{
    Q_D(QIviMediaPlayer);
    d->init();
}

bool QIviMediaPlayer::acceptServiceObject(QIviServiceObject *serviceObject)
{
    return serviceObject->interfaces().contains(QIviStringMediaPlayerInterfaceName);
}

void QIviMediaPlayer::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIviMediaPlayer);

    QIviMediaPlayerBackendInterface *backend = d->playerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::positionChanged,
                            d, &QIviMediaPlayerPrivate::onPositionChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::currentTrackChanged,
                            d, &QIviMediaPlayerPrivate::onCurrentTrackChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::durationChanged,
                            d, &QIviMediaPlayerPrivate::onDurationChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::dataFetched,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onDataFetched);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::countChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onCountChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::dataChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onDataChanged);
    QObjectPrivate::connect(backend, &QIviMediaPlayerBackendInterface::currentIndexChanged,
                            d->m_playQueue->d_func(), &QIviPlayQueuePrivate::onCurrentIndexChanged);

    backend->initialize();
    d->m_playQueue->d_func()->resetModel();
}

void QIviMediaPlayer::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviMediaPlayerBackendInterface *backend = qobject_cast<QIviMediaPlayerBackendInterface*>(serviceObject->interfaceInstance(QIviStringMediaPlayerInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

void QIviMediaPlayer::clearServiceObject()
{
    Q_D(QIviMediaPlayer);
    d->clearToDefaults();
}

#include "moc_qivimediaplayer.cpp"
