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
