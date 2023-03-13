// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef DATABASE_HELPER_H
#define DATABASE_HELPER_H

#include <QByteArray>
#include <QString>
#include <QDir>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QTemporaryFile>
#include <QCoreApplication>

#include "logging.h"

QString mediaDatabaseFile(const QVariantMap &serviceSettings)
{
    QString dbFile;
    bool useTemporaryDatabase = serviceSettings.value(QStringLiteral("useTemporaryDatabase")).toBool();
    QString database = serviceSettings.value(QStringLiteral("database")).toString();
    if (qEnvironmentVariableIsSet("QTIFMEDIA_TEMPORARY_DATABASE")) {
        qCInfo(media) << "QTIFMEDIA_TEMPORARY_DATABASE environment variable is set.\n"
                      << "Overriding service setting: 'useTemporaryDatabas'";
        useTemporaryDatabase = true;
    } else if (qEnvironmentVariableIsSet("QTIFMEDIA_SIMULATOR_DATABASE")) {
        qCInfo(media) << "QTIFMEDIA_SIMULATOR_DATABASE environment variable is set.\n"
                      << "Overriding service setting: 'database'";
        database = QFile::decodeName(qgetenv("QTIFMEDIA_SIMULATOR_DATABASE"));
    }

    if (useTemporaryDatabase) {
        auto *tempFile = new QTemporaryFile(qApp);
        tempFile->open();
        dbFile = tempFile->fileName();
        qCInfo(media) << "Using the temporary database: " << tempFile->fileName();
    } else if (!database.isEmpty()) {
        dbFile = database;
        if (!QFileInfo(dbFile).isAbsolute())
            qCInfo(media) << "Please set an valid absolute path for the service setting 'database'. Current path:" << dbFile;
    } else {
        const QDir cacheLocation = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
        if (!cacheLocation.exists())
            cacheLocation.mkpath(QStringLiteral("."));
        dbFile = cacheLocation.absoluteFilePath(QStringLiteral("ifmedia.db"));
        qCInfo(media) << "Used media database:" << dbFile;
    }
    return dbFile;
}

QSqlDatabase createDatabaseConnection(const QString &connectionName, const QString &dbFile)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), connectionName);
    db.setDatabaseName(dbFile);
    if (!db.open())
        qFatal("Couldn't couldn't open database: %s", qPrintable(db.lastError().text()));
    return db;
}

void createMediaDatabase(const QString &dbFile)
{
    QSqlDatabase db = createDatabaseConnection(QStringLiteral("main"), dbFile);
    QSqlQuery createQueue(db);
    createQueue.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS \"queue\" (\"id\" INTEGER PRIMARY KEY, \"qindex\" INTEGER, \"track_index\" INTEGER)"));
    if (createQueue.lastError().isValid())
        qFatal("Couldn't create Database Tables: %s", qPrintable(createQueue.lastError().text()));

    QSqlQuery createTrack(db);
    createTrack.exec("CREATE TABLE IF NOT EXISTS track "
                     "(id integer primary key, "
                     "trackName varchar(200), "
                     "albumName varchar(200), "
                     "artistName varchar(200), "
                     "genre varchar(200), "
                     "number integer,"
                     "file varchar(200),"
                     "coverArtUrl varchar(200),"
                     "UNIQUE(file))");

    if (createTrack.lastError().isValid())
        qFatal("Couldn't create Database Tables: %s", qPrintable(createTrack.lastError().text()));
}

#endif // DATABASE_HELPER_H
