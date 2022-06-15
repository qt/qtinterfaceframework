// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef MEDIAINDEXERBACKEND_H
#define MEDIAINDEXERBACKEND_H

#include <QtIfMedia/QIfMediaIndexerControl>
#include <QtIfMedia/QIfMediaIndexerControlBackendInterface>

#include <QFutureWatcher>
#include <QQueue>
#include <QSqlDatabase>

QT_FORWARD_DECLARE_CLASS(QThreadPool);

class MediaIndexerBackend : public QIfMediaIndexerControlBackendInterface
{
    Q_OBJECT

    struct ScanData;
public:
    explicit MediaIndexerBackend(const QSqlDatabase &database, QObject *parent = nullptr);

    void initialize() override;
    void pause() override;
    void resume() override;

    qreal progress() const;
    QIfMediaIndexerControl::State state() const;

signals:
    void indexingDone();
    void removeFromQueue(int index);

public slots:
    void addMediaFolder(const QString &path);
    void removeMediaFolder(const QString &path);

private slots:
    bool scanWorker(const MediaIndexerBackend::ScanData &scanData);
    void onScanFinished();

private:
    void scanNext();
    void setProgress(qreal progress);
    void setState(QIfMediaIndexerControl::State state);

    QSqlDatabase m_db;
    struct ScanData {
        enum Operation {
            Verify,
            Add,
            Remove
        };
        Operation operation = Add;
        QString folder;
    };

    qreal m_progress;
    QIfMediaIndexerControl::State m_state;
    QQueue<ScanData> m_folderQueue;
    QFutureWatcher<bool> m_watcher;
    QThreadPool *m_threadPool;
};

#endif // MEDIAINDEXERBACKEND_H
