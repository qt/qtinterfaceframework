// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QGuiApplication>
#include <QDir>
#include <QLockFile>
#include <QIfConfiguration>

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

using namespace Qt::StringLiterals;

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
    QLockFile lockFile(u"%1/%2.lock"_s.arg(QDir::tempPath(), app.applicationName()));
    if (!lockFile.tryLock(100)) {
        qCritical("%s already running, aborting...", qPrintable(app.applicationName()));
        return EXIT_FAILURE;
    }

    QVariantMap serviceSettings = QIfConfiguration::serviceSettings(u"ifmedia"_s);
    QString dbFile = mediaDatabaseFile(serviceSettings);
    createMediaDatabase(dbFile);

    MediaIndexerBackend *indexerBackend = new MediaIndexerBackend(serviceSettings, createDatabaseConnection(u"indexer"_s, dbFile), qApp);
    MediaPlayerBackend *playerBackend = new MediaPlayerBackend(serviceSettings, createDatabaseConnection(u"player"_s, dbFile), qApp);
    MediaDiscoveryBackend *discoveryBackend = new MediaDiscoveryBackend(serviceSettings, qApp);
    SearchAndBrowseBackend *searchAndBrowseBackend = new SearchAndBrowseBackend(serviceSettings, createDatabaseConnection(u"model"_s, dbFile), qApp);

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
