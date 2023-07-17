// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "logging.h"
#include "mediadiscoverybackend.h"
#include "usbdevice.h"

#include <QDir>
#include <QTimer>
#include <QtDebug>

using namespace Qt::StringLiterals;

MediaDiscoveryBackend::MediaDiscoveryBackend(const QVariantMap &serviceSettings, QObject *parent)
    : QIfMediaDeviceDiscoveryModelBackendInterface(parent)
{
    m_deviceFolder = QDir::homePath() + "/usb-simulation";

    QString customDeviceFolder = serviceSettings.value(u"customDeviceFolder"_s).toString();
    if (qEnvironmentVariableIsSet("QTIFMEDIA_SIMULATOR_DEVICEFOLDER")) {
        qCInfo(media) << "QTIFMEDIA_SIMULATOR_DEVICEFOLDER environment variable is set.\n"
                      << "Overriding service setting: 'customDeviceFolder'";
        customDeviceFolder = qgetenv("QTIFMEDIA_SIMULATOR_DEVICEFOLDER");
    }

    if (customDeviceFolder.isEmpty())
        qCInfo(media) << "The service setting 'customDeviceFolder' is not set, falling back to:" << m_deviceFolder;
    else
        m_deviceFolder = customDeviceFolder;

    const QDir deviceFolder(m_deviceFolder);
    const QStringList folders = deviceFolder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders) {
        qCDebug(media) << "Adding USB Device for: " << folder;
        m_deviceMap.insert(folder, new USBDevice(deviceFolder.absoluteFilePath(folder)));
    }
}

void MediaDiscoveryBackend::initialize()
{
#ifndef QT_NO_FILESYSTEMWATCHER
    m_watcher.addPath(m_deviceFolder);
    connect(&m_watcher, &QFileSystemWatcher::directoryChanged, this, &MediaDiscoveryBackend::onDirectoryChanged);
#endif

    emit availableDevices(m_deviceMap.values());
    emit initializationDone();
}

QMap<QString, QIfServiceObject*> MediaDiscoveryBackend::deviceMap() const
{
    return m_deviceMap;
}

void MediaDiscoveryBackend::onDirectoryChanged(const QString &path)
{
    Q_UNUSED(path)
    QDir deviceFolder(m_deviceFolder);

    //Check for removed Devices
    for (auto i = m_deviceMap.cbegin(), end = m_deviceMap.cend(); i != end; ++i) {
        const QString &folder = i.key();
        if (!deviceFolder.exists(folder)) {
            qCDebug(media) << "Removing USB Device for: " << folder;
            QIfServiceObject *device = m_deviceMap.take(folder);
            emit deviceRemoved(device);
            emit mediaDirectoryRemoved(deviceFolder.absoluteFilePath(folder));
        }
    }

    //Check for newly added Devices
    const QStringList folders = deviceFolder.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &folder : folders) {
        if (m_deviceMap.contains(folder))
            continue;

        qCDebug(media) << "Adding USB Device for: " << folder;
        USBDevice *device = new USBDevice(deviceFolder.absoluteFilePath(folder));
        m_deviceMap.insert(folder, device);
        emit deviceAdded(device);
        const QString absFolder = deviceFolder.absoluteFilePath(folder);
        // If we point the simulation to a real mount location, give the mount some time to actually make
        // the files accessible
        QTimer::singleShot(2000, this, [this, absFolder](){emit mediaDirectoryAdded(absFolder);});
    }
}

#include "moc_mediadiscoverybackend.cpp"
