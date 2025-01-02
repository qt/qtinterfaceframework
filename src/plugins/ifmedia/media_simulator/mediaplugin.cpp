// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "logging.h"
#include "mediadiscoverybackend.h"
#include "mediaindexerbackend.h"
#include "mediaplayerbackend.h"
#include "mediaplugin.h"
#include "searchandbrowsebackend.h"
#include "usbdevice.h"
#include "database_helper.h"

#include <QtInterfaceFramework/QIfFilterAndBrowseModel>
#include <QtIfMedia/QIfMediaPlayer>

#include <QCoreApplication>
#include <QStringList>
#include <QtDebug>

MediaSimulatorPlugin::MediaSimulatorPlugin(QObject *parent)
    : QObject(parent)
    , m_player(nullptr)
    , m_browse(nullptr)
    , m_discovery(nullptr)
    , m_indexer(nullptr)
{
}

void MediaSimulatorPlugin::createInstances()
{
    QString dbFile = mediaDatabaseFile(m_serviceSettings);

    createMediaDatabase(dbFile);

    m_player = new MediaPlayerBackend(m_serviceSettings, createDatabaseConnection(QStringLiteral("player"), dbFile), this);
    m_browse = new SearchAndBrowseBackend(m_serviceSettings, createDatabaseConnection(QStringLiteral("model"), dbFile), this);
    m_indexer = new MediaIndexerBackend(m_serviceSettings, createDatabaseConnection(QStringLiteral("indexer"), dbFile), this);
    m_discovery = new MediaDiscoveryBackend(m_serviceSettings, this);

    auto deviceMap = m_discovery->deviceMap();
    for (auto it = deviceMap.cbegin(); it != deviceMap.cend(); it++) {
        USBDevice *device = qobject_cast<USBDevice*>(it.value());
        if (!device)
            continue;
        m_indexer->addMediaFolder(device->folder());
    }

    QObject::connect(m_indexer, &MediaIndexerBackend::removeFromQueue,
                     m_player, &MediaPlayerBackend::remove);
    QObject::connect(m_discovery, &MediaDiscoveryBackend::mediaDirectoryAdded,
                     m_indexer, &MediaIndexerBackend::addMediaFolder);
    QObject::connect(m_discovery, &MediaDiscoveryBackend::mediaDirectoryRemoved,
                     m_indexer, &MediaIndexerBackend::removeMediaFolder);

}

QStringList MediaSimulatorPlugin::interfaces() const
{
    QStringList list;
    list << QStringLiteral(QIfMediaPlayer_iid);
    list << QStringLiteral(QIfFilterAndBrowseModel_iid);
    list << QStringLiteral(QIfMediaDeviceDiscovery_iid);
    list << QStringLiteral(QIfMediaIndexer_iid);
    return list;
}

QIfFeatureInterface *MediaSimulatorPlugin::interfaceInstance(const QString &interface) const
{
    if (!m_player)
        const_cast<MediaSimulatorPlugin*>(this)->createInstances();

    if (interface == QStringLiteral(QIfMediaPlayer_iid))
        return m_player;
    else if (interface == QStringLiteral(QIfFilterAndBrowseModel_iid))
        return m_browse;
    else if (interface == QStringLiteral(QIfMediaDeviceDiscovery_iid))
        return m_discovery;
    else if (interface == QStringLiteral(QIfMediaIndexer_iid))
        return m_indexer;

    return nullptr;
}

QString MediaSimulatorPlugin::configurationId() const
{
    return QStringLiteral("qtifmedia");
}

void MediaSimulatorPlugin::updateServiceSettings(const QVariantMap &settings)
{
    qCDebug(media) << Q_FUNC_INFO << settings;
    m_serviceSettings = settings;
}
