// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "mediaplayerbackend.h"
#include "qifqmlconversion_helper.h"

#include <QTimer>
#include <QSettings>

Q_LOGGING_CATEGORY(qLcROQIfMediaPlayer, "interfaceframework.media.qifmediaplayerbackend.remoteobjects", QtDebugMsg)

MediaPlayerBackend::MediaPlayerBackend(QObject *parent)
    : QIfMediaPlayerBackendInterface(parent)
    , m_node(nullptr)
    , m_helper(new QIfRemoteObjectsReplicaHelper(qLcROQIfMediaPlayer(), this))
{
    qRegisterMetaType<QIfPlayableItem>();
    qRegisterMetaType<QIfAudioTrackItem>();
}

void MediaPlayerBackend::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized()) {
        emit canReportCountChanged(m_replica->canReportCount());
        emit playModeChanged(m_replica->playMode());
        emit playStateChanged(m_replica->playState());
        emit positionChanged(m_replica->position());
        emit durationChanged(m_replica->duration());
        emit currentTrackChanged(m_helper->fromRemoteObjectVariant(m_replica->currentTrack()));
        emit currentIndexChanged(m_replica->currentIndex());
        emit volumeChanged(m_replica->volume());
        emit mutedChanged(m_replica->muted());
        emit initializationDone();
    }

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIfMediaPlayer) << "QtIfMedia.QIfMediaPlayer wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void MediaPlayerBackend::play()
{
    m_replica->play();
}

void MediaPlayerBackend::pause()
{
    m_replica->pause();
}

void MediaPlayerBackend::stop()
{
    m_replica->stop();
}

void MediaPlayerBackend::seek(qint64 offset)
{
    m_replica->seek(offset);
}

void MediaPlayerBackend::next()
{
    m_replica->next();
}

void MediaPlayerBackend::previous()
{
    m_replica->previous();
}

void MediaPlayerBackend::setPlayMode(QIfMediaPlayer::PlayMode playMode)
{
    m_replica->pushPlayMode(playMode);
}

void MediaPlayerBackend::setPosition(qint64 position)
{
    m_replica->pushPosition(position);
}

void MediaPlayerBackend::setCurrentIndex(int currentIndex)
{
    m_replica->pushCurrentIndex(currentIndex);
}

void MediaPlayerBackend::setVolume(int volume)
{
    m_replica->pushVolume(volume);
}

void MediaPlayerBackend::setMuted(bool muted)
{
    m_replica->pushMuted(muted);
}

void MediaPlayerBackend::fetchData(const QUuid &identifier, int start, int count)
{
    m_replica->fetchData(identifier, start, count);
}

void MediaPlayerBackend::insert(int index, const QVariant &item)
{
    m_replica->insert(index, item);
}

void MediaPlayerBackend::remove(int index)
{
    m_replica->remove(index);
}

void MediaPlayerBackend::move(int currentIndex, int newIndex)
{
    m_replica->move(currentIndex, newIndex);
}

bool MediaPlayerBackend::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcROQIfMediaPlayer) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("qtifmedia"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:qtifmedia")).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode(this);
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIfMediaPlayer) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcROQIfMediaPlayer) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIfMediaPlayerReplica>(QStringLiteral("QtIfMedia.QIfMediaPlayer")));
        setupConnections();
    }
    return true;
}

void MediaPlayerBackend::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIfRemoteObjectsReplicaHelper::onNodeError);
    connect(m_helper, &QIfRemoteObjectsReplicaHelper::errorChanged, this, &QIfFeatureInterface::errorChanged);

    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIfRemoteObjectsReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIfFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIfMediaPlayerReplica::playModeChanged, this, &MediaPlayerBackend::playModeChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::playStateChanged, this, &MediaPlayerBackend::playStateChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::positionChanged, this, &MediaPlayerBackend::positionChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::durationChanged, this, &MediaPlayerBackend::durationChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::currentTrackChanged, this, [this] (const QVariant &currentTrack) {
        emit currentTrackChanged(m_helper->fromRemoteObjectVariant(currentTrack));
    });
    connect(m_replica.data(), &QIfMediaPlayerReplica::currentIndexChanged, this, &MediaPlayerBackend::currentIndexChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::volumeChanged, this, &MediaPlayerBackend::volumeChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::mutedChanged, this, &MediaPlayerBackend::mutedChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::countChanged, this, &MediaPlayerBackend::countChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::canReportCountChanged, this, &MediaPlayerBackend::canReportCountChanged);
    connect(m_replica.data(), &QIfMediaPlayerReplica::dataFetched, this, &MediaPlayerBackend::dataFetched);
    connect(m_replica.data(), &QIfMediaPlayerReplica::dataChanged, this, &MediaPlayerBackend::dataChanged);
}
