// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#ifndef MEDIAPLUGIN_H
#define MEDIAPLUGIN_H

#include <QtInterfaceFramework/QIfServiceInterface>

#include <QSqlDatabase>

class MediaPlayerBackend;
class SearchAndBrowseBackend;
class MediaDiscoveryBackend;
class MediaIndexerBackend;
class AmFmTunerBackend;

class MediaSimulatorPlugin : public QObject, QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "media_simulator.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit MediaSimulatorPlugin(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

    QString configurationId() const override;
    void updateServiceSettings(const QVariantMap &settings) override;

private:
    void createInstances();

    MediaPlayerBackend *m_player;
    SearchAndBrowseBackend *m_browse;
    MediaDiscoveryBackend *m_discovery;
    MediaIndexerBackend *m_indexer;
    QVariantMap m_serviceSettings;
};

#endif // MEDIAPLUGIN_H
