/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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

#include <QGuiApplication>
#include <QDir>
#include <QLockFile>

#include "database_helper.h"

#include "mediaplayerbackend.h"
#include "qifmediaplayerqtroadapter.h"

#include "mediaindexerbackend.h"
#include "qifmediaindexerqtroadapter.h"

#include "mediadiscoverybackend.h"
#include "qifmediadiscoveryqtroadapter.h"

#include "searchandbrowsebackend.h"
#include "qiffilterandbrowsemodelqtroadapter.h"
#include "usbdevice.h"

#include "core.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    // QtMultimedia doesn't work with a QCoreApplication on macos
    // Use the env variable to prevent Qt from creating an app icon in the dock
    qputenv("QT_MAC_DISABLE_FOREGROUND_APPLICATION_TRANSFORM", "1");
    QGuiApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif

    // single instance guard
    QLockFile lockFile(QStringLiteral("%1/%2.lock").arg(QDir::tempPath(), app.applicationName()));
    if (!lockFile.tryLock(100)) {
        qCritical("%s already running, aborting...", qPrintable(app.applicationName()));
        return EXIT_FAILURE;
    }

    QString dbFile = mediaDatabaseFile();
    createMediaDatabase(dbFile);

    MediaIndexerBackend *indexerBackend = new MediaIndexerBackend(createDatabaseConnection(QStringLiteral("indexer"), dbFile), qApp);
    MediaPlayerBackend *playerBackend = new MediaPlayerBackend(createDatabaseConnection(QStringLiteral("player"), dbFile), qApp);
    MediaDiscoveryBackend *discoveryBackend = new MediaDiscoveryBackend(qApp);
    SearchAndBrowseBackend *searchAndBrowseBackend = new SearchAndBrowseBackend(createDatabaseConnection(QStringLiteral("model"), dbFile), qApp);

    auto deviceMap = discoveryBackend->deviceMap();
    for (auto it = deviceMap.cbegin(); it != deviceMap.cend(); it++) {
        USBDevice *device = qobject_cast<USBDevice*>(it.value());
        if (!device)
            continue;
        indexerBackend->addMediaFolder(device->folder());
    }

    QObject::connect(indexerBackend, &MediaIndexerBackend::removeFromQueue,
                     playerBackend, &MediaPlayerBackend::remove);
    QObject::connect(discoveryBackend, &MediaDiscoveryBackend::mediaDirectoryAdded,
                     indexerBackend, &MediaIndexerBackend::addMediaFolder);
    QObject::connect(discoveryBackend, &MediaDiscoveryBackend::mediaDirectoryRemoved,
                     indexerBackend, &MediaIndexerBackend::removeMediaFolder);

    //initialize all our backends
    indexerBackend->initialize();
    playerBackend->initialize();
    discoveryBackend->initialize();
    searchAndBrowseBackend->initialize();

    //Start Remoting the backends
    Core::instance()->host()->enableRemoting<QIfMediaIndexerAddressWrapper>(new QIfMediaIndexerQtRoAdapter(indexerBackend));
    Core::instance()->host()->enableRemoting<QIfMediaPlayerAddressWrapper>(new QIfMediaPlayerQtRoAdapter(playerBackend));
    Core::instance()->host()->enableRemoting<QIfMediaDiscoveryModelAddressWrapper>(new QIfMediaDiscoveryModelQtRoAdapter(discoveryBackend));
    Core::instance()->host()->enableRemoting<QIfFilterAndBrowseModelAddressWrapper>(new QIfFilterAndBrowseModelQtRoAdapter(searchAndBrowseBackend));

    return app.exec();
}
