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
