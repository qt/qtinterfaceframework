// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef MEDIADISCOVERYBACKEND_H
#define MEDIADISCOVERYBACKEND_H

#include <QtIfMedia/QIfMediaDeviceDiscoveryModel>
#include <QtIfMedia/QIfMediaDeviceDiscoveryModelBackendInterface>

#include <QFileSystemWatcher>

class MediaDiscoveryBackend : public QIfMediaDeviceDiscoveryModelBackendInterface
{
    Q_OBJECT

public:
    MediaDiscoveryBackend(QObject *parent = nullptr);

    void initialize() override;

    QMap<QString, QIfServiceObject*> deviceMap() const;

private slots:
    void onDirectoryChanged(const QString &path);

signals:
    void mediaDirectoryAdded(const QString &path);
    void mediaDirectoryRemoved(const QString &path);

private:
    QString m_deviceFolder;
#ifndef QT_NO_FILESYSTEMWATCHER
    QFileSystemWatcher m_watcher;
#endif
    QMap<QString, QIfServiceObject*> m_deviceMap;
};

#endif // MEDIADISCOVERYBACKEND_H
