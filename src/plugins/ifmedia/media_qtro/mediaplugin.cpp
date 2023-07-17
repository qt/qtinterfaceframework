// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "mediaplugin.h"
#include "mediaplayerbackend.h"
#include "mediaindexerbackend.h"
#include "filterandbrowsemodel.h"
#include "mediadiscoverybackend.h"

#include <QtIfMedia/QIfMediaPlayer>

#include <QCoreApplication>
#include <QStringList>
#include <QtDebug>
#include <QSettings>
#include <QRemoteObjectNode>

using namespace Qt::StringLiterals;

MediaQtROPlugin::MediaQtROPlugin(QObject *parent)
    : QObject(parent)
    , m_player(new MediaPlayerBackend(this))
    , m_indexer(new MediaIndexerBackend(this))
    , m_searchModel(new FilterAndBrowseModel(this))
    , m_discovery(new MediaDiscoveryBackend(this))
{
}

QStringList MediaQtROPlugin::interfaces() const
{
    QStringList list;

    list << QStringLiteral(QIfMediaPlayer_iid);
    list << QStringLiteral(QIfMediaIndexer_iid);
    list << QStringLiteral(QIfFilterAndBrowseModel_iid);
    list << QStringLiteral(QIfMediaDeviceDiscovery_iid);
    return list;
}

QIfFeatureInterface *MediaQtROPlugin::interfaceInstance(const QString &interface) const
{
    if (interface == QStringLiteral(QIfMediaPlayer_iid))
        return m_player;
    else if (interface == QStringLiteral(QIfMediaIndexer_iid))
        return m_indexer;
    else if (interface == QStringLiteral(QIfFilterAndBrowseModel_iid))
        return m_searchModel;
    else if (interface == QStringLiteral(QIfMediaDeviceDiscovery_iid))
        return m_discovery;

    return nullptr;
}

QString MediaQtROPlugin::configurationId() const
{
    return u"qtifmedia"_s;
}

void MediaQtROPlugin::updateServiceSettings(const QVariantMap &settings)
{
    m_player->updateServiceSettings(settings);
    m_indexer->updateServiceSettings(settings);
    m_searchModel->updateServiceSettings(settings);
    m_discovery->updateServiceSettings(settings);
}

#include "moc_mediaplugin.cpp"
