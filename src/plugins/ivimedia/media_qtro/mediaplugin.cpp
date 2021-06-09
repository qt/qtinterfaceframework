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
