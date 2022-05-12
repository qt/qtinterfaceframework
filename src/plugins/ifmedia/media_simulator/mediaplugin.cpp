/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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
    , m_discovery(new MediaDiscoveryBackend(this))
{
    QString dbFile = mediaDatabaseFile();

    createMediaDatabase(dbFile);

    m_player = new MediaPlayerBackend(createDatabaseConnection(QStringLiteral("player"), dbFile), this);
    m_browse = new SearchAndBrowseBackend(createDatabaseConnection(QStringLiteral("model"), dbFile), this);
    m_indexer = new MediaIndexerBackend(createDatabaseConnection(QStringLiteral("indexer"), dbFile), this);

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
