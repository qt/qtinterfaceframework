// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "mediaindexerbackend.h"
#include "logging.h"

#include <QtConcurrent/QtConcurrent>

#include <QDirIterator>
#include <QImage>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QThreadPool>
#include <QtDebug>

#ifndef QTIF_NO_TAGLIB
#include <attachedpictureframe.h>
#include <fileref.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <id3v2tag.h>
#include <mpegfile.h>
#include <tag.h>
#include <taglib.h>
#include <tstring.h>
#endif

using namespace Qt::StringLiterals;

MediaIndexerBackend::MediaIndexerBackend(const QVariantMap &serviceSettings, const QSqlDatabase &database, QObject *parent)
    : QIfMediaIndexerControlBackendInterface(parent)
    , m_db(database)
    , m_state(QIfMediaIndexerControl::Idle)
    , m_threadPool(new QThreadPool(this))
{
    m_threadPool->setMaxThreadCount(1);

    connect(&m_watcher, &QFutureWatcherBase::finished, this, &MediaIndexerBackend::onScanFinished);

    QString customMediaFolder = serviceSettings.value(u"customMediaFolder"_s).toString();
    if (qEnvironmentVariableIsSet("QTIFMEDIA_SIMULATOR_LOCALMEDIAFOLDER")) {
        qCInfo(media) << "QTIFMEDIA_SIMULATOR_LOCALMEDIAFOLDER environment variable is set.\n"
                      << "Overriding service setting: 'customMediaFolder'";
        customMediaFolder = qgetenv("QTIFMEDIA_SIMULATOR_LOCALMEDIAFOLDER");
    }

    QStringList mediaFolderList;
    if (!customMediaFolder.isEmpty()) {
        qCInfo(media) << "The service setting 'customMediaFolder' is set to:" << customMediaFolder;
        mediaFolderList.append(customMediaFolder);
    } else {
        mediaFolderList = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
        qCInfo(media) << "Searching for music files in the following locations: " << mediaFolderList;
    }

#ifdef QTIF_NO_TAGLIB
    qCCritical(media) << "The indexer simulation doesn't work without an installed taglib";
#endif

    ScanData data;
    data.operation = ScanData::Verify;
    m_folderQueue.append(data);
    scanNext();

    //We want to have the indexer running also when the Indexing interface is not used.
    for (const QString &folder : std::as_const(mediaFolderList))
        addMediaFolder(folder);
}

void MediaIndexerBackend::initialize()
{
    emit stateChanged(m_state);
    emit initializationDone();
}

void MediaIndexerBackend::pause()
{
    static const QLatin1String error("SIMULATION: Pausing the indexing is not supported");
    qCWarning(media) << error;
    emit errorChanged(QIfAbstractFeature::InvalidOperation, error);
}

void MediaIndexerBackend::resume()
{
    static const QLatin1String error("SIMULATION: Resuming the indexing is not supported");
    qCWarning(media) << error;
    emit errorChanged(QIfAbstractFeature::InvalidOperation, error);
}

qreal MediaIndexerBackend::progress() const
{
    return m_progress;
}

QIfMediaIndexerControl::State MediaIndexerBackend::state() const
{
    return m_state;
}

void MediaIndexerBackend::addMediaFolder(const QString &path)
{
    ScanData data;
    data.operation = ScanData::Add;
    data.folder = path;
    m_folderQueue.append(data);

    scanNext();
}

void MediaIndexerBackend::removeMediaFolder(const QString &path)
{
    ScanData data;
    data.operation = ScanData::Remove;
    data.folder = path;
    m_folderQueue.append(data);

    scanNext();
}

bool MediaIndexerBackend::scanWorker(const ScanData &scanData)
{
    setState(QIfMediaIndexerControl::Active);

    auto removeDataFunc = [this](QSqlQuery &query, const QStringList &ids) {
        const QString idsToRemove = ids.join(u", "_s);
        if (!query.exec(u"DELETE from queue WHERE track_index IN (%1)"_s.arg(idsToRemove))) {
            setState(QIfMediaIndexerControl::Error);
            sqlError(this, query.lastQuery(), query.lastError().text());
            return false;
        }
        if (!query.exec(u"DELETE from track WHERE id IN (%1)"_s.arg(idsToRemove))) {
            setState(QIfMediaIndexerControl::Error);
            sqlError(this, query.lastQuery(), query.lastError().text());
            return false;
        }
        return true;
    };

    if (scanData.operation == ScanData::Verify) {
        qCInfo(media) << "Checking Database";
        QSqlQuery query(m_db);

        QStringList idsToRemove;
        bool ret = query.exec(u"SELECT track.id, queue.qindex, track.file FROM track "
                                             "LEFT JOIN queue ON queue.track_index=track.id"_s);
        if (ret) {
            while (query.next()) {
                if (!QFile::exists(query.value(2).toString())) {
                    qCInfo(media) << "Removing stale track: " << query.value(2).toString();
                    idsToRemove.append(query.value(0).toString());
                    if (!query.value(1).isNull())
                        emit removeFromQueue(query.value(1).toInt());
                }
            }

            if (!removeDataFunc(query, idsToRemove))
                return false;

            m_db.commit();
            return true;
        } else {
            setState(QIfMediaIndexerControl::Error);
            sqlError(this, query.lastQuery(), query.lastError().text());
            return false;
        }
    } else if (scanData.operation == ScanData::Remove) {
        qCInfo(media) << "Removing content: " << scanData.folder;
        QSqlQuery query(m_db);

        QStringList idsToRemove;
        bool ret = query.exec(u"SELECT track.id, queue.qindex FROM track "
                              "LEFT JOIN queue ON queue.track_index=track.id "
                              "WHERE file LIKE '%1%'"_s.arg(scanData.folder));
        if (ret) {
            while (query.next()) {
                idsToRemove.append(query.value(0).toString());
                if (!query.value(1).isNull())
                    emit removeFromQueue(query.value(1).toInt());
            }
        } else {
            setState(QIfMediaIndexerControl::Error);
            sqlError(this, query.lastQuery(), query.lastError().text());
            return false;
        }

        if (!removeDataFunc(query, idsToRemove))
            return false;

        m_db.commit();
        return true;
    }

    qCInfo(media) << "Scanning path: " << scanData.folder;

    QStringList mediaFiles{u"*.mp3"_s};

    QVector<QString> files;
    QDirIterator it(scanData.folder, mediaFiles, QDir::Files, QDirIterator::Subdirectories);
    qCInfo(media) << "Calculating total file count";

    while (it.hasNext())
        files.append(it.next());
    qsizetype totalFileCount = files.size();
    qCInfo(media) << "total files: " << totalFileCount;
    int currentFileIndex = 0;
    for (const QString &fileName : std::as_const(files)) {
        qCInfo(media) << "Processing file:" << fileName;

        if (qApp->closingDown())
            return false;

        QString defaultCoverArtUrl = fileName + u".png"_s;
        QString coverArtUrl;
#ifndef QTIF_NO_TAGLIB
        TagLib::FileRef f(TagLib::FileName(QFile::encodeName(fileName)));
        if (f.isNull())
            continue;
        QString trackName = TStringToQString(f.tag()->title());
        QString albumName = TStringToQString(f.tag()->album());
        QString artistName = TStringToQString(f.tag()->artist());
        QString genre = TStringToQString(f.tag()->genre());
        unsigned int number = f.tag()->track();

        // Extract cover art
        if (fileName.endsWith(QLatin1String("mp3"))) {
            auto *file = static_cast<TagLib::MPEG::File*>(f.file());
            TagLib::ID3v2::Tag *tag = file->ID3v2Tag(true);
            TagLib::ID3v2::FrameList frameList = tag->frameList("APIC");

            if (frameList.isEmpty()) {
                qCWarning(media) << "No cover art was found";
            } else if (!QFile::exists(defaultCoverArtUrl)) {
                auto *coverImage = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frameList.front());

                QImage coverQImg;
                coverArtUrl = defaultCoverArtUrl;

                coverQImg.loadFromData(reinterpret_cast<const uchar *>(coverImage->picture().data()), int(coverImage->picture().size()));
                coverQImg.save(coverArtUrl, "PNG");
            } else {
                coverArtUrl = defaultCoverArtUrl;
            }
        }

        QSqlQuery query(m_db);

        query.prepare("INSERT OR IGNORE INTO track (trackName, albumName, artistName, genre, number, file, coverArtUrl) "
                      "VALUES (:trackName, :albumName, :artistName, :genre, :number, :file, :coverArtUrl)");

        query.bindValue(u":trackName"_s, trackName);
        query.bindValue(u":albumName"_s, albumName);
        query.bindValue(u":artistName"_s, artistName);
        query.bindValue(u":genre"_s, genre);
        query.bindValue(u":number"_s, number);
        query.bindValue(u":file"_s, fileName);
        query.bindValue(u":coverArtUrl"_s, coverArtUrl);

        bool ret = query.exec();

        if (!ret) {
            setState(QIfMediaIndexerControl::Error);
            sqlError(this, query.lastQuery(), query.lastError().text());
            return false;
        } else {
            setProgress(qreal(++currentFileIndex)/qreal(totalFileCount));
        }
#else
        setProgress(qreal(++currentFileIndex)/qreal(totalFileCount));
#endif // QTIF_NO_TAGLIB
    }

    return true;
}

void MediaIndexerBackend::onScanFinished()
{
    if (!m_folderQueue.isEmpty()) {
        scanNext();
        return;
    }

    qCInfo(media) << "Scanning done";
#ifdef QTIF_NO_TAGLIB
    qCCritical(media) << "No data was added, this is just a simulation";
#endif
    setProgress(1);
    emit indexingDone();


    //If the last run didn't succeed we will stay in the Error state
    if (m_watcher.future().result())
        setState(QIfMediaIndexerControl::Idle);
}

void MediaIndexerBackend::scanNext()
{
    if (m_watcher.isRunning())
        return;

    ScanData data = m_folderQueue.dequeue();
    m_watcher.setFuture(QtConcurrent::run(&MediaIndexerBackend::scanWorker, this, data));
}

void MediaIndexerBackend::setProgress(qreal progress)
{
    m_progress = progress;
    emit progressChanged(progress);
}

void MediaIndexerBackend::setState(QIfMediaIndexerControl::State state)
{
    m_state = state;
    emit stateChanged(state);
}

#include "moc_mediaindexerbackend.cpp"
