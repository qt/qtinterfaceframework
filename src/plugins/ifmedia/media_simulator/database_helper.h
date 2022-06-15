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

QString mediaDatabaseFile()
{
    QString dbFile;
    const QByteArray database = qgetenv("QTIFMEDIA_SIMULATOR_DATABASE");
    if (qEnvironmentVariableIsSet("QTIFMEDIA_TEMPORARY_DATABASE")) {
        auto *tempFile = new QTemporaryFile(qApp);
        tempFile->open();
        dbFile = tempFile->fileName();
        qCInfo(media) << "QTIFMEDIA_TEMPORARY_DATABASE environment variable is set.\n"
                    << "Using the temporary database: " << tempFile->fileName();
    } else if (!database.isEmpty()) {
        dbFile = QFile::decodeName(database);
        if (!QFileInfo(dbFile).isAbsolute())
            qCInfo(media) << "Please set an valid absolute path for QTIFMEDIA_SIMULATOR_DATABASE. Current path:" << dbFile;
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
    QSqlQuery query = db.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS \"queue\" (\"id\" INTEGER PRIMARY KEY, \"qindex\" INTEGER, \"track_index\" INTEGER)"));
    if (query.lastError().isValid())
        qFatal("Couldn't create Database Tables: %s", qPrintable(query.lastError().text()));

    query = db.exec("CREATE TABLE IF NOT EXISTS track "
                     "(id integer primary key, "
                     "trackName varchar(200), "
                     "albumName varchar(200), "
                     "artistName varchar(200), "
                     "genre varchar(200), "
                     "number integer,"
                     "file varchar(200),"
                     "coverArtUrl varchar(200),"
                     "UNIQUE(file))");

    if (query.lastError().isValid())
        qFatal("Couldn't create Database Tables: %s", qPrintable(query.lastError().text()));
    db.commit();
}

#endif // DATABASE_HELPER_H
