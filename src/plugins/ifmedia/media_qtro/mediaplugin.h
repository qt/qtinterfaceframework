// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef MEDIAPLUGIN_H
#define MEDIAPLUGIN_H

#include <QtInterfaceFramework/QIfServiceInterface>

class MediaPlayerBackend;
class MediaIndexerBackend;
class FilterAndBrowseModel;
class MediaDiscoveryBackend;

class MediaQtROPlugin : public QObject, QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "media_qtro.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit MediaQtROPlugin(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

    QString configurationId() const override;
    void updateServiceSettings(const QVariantMap &settings) override;

private:

    MediaPlayerBackend *m_player;
    MediaIndexerBackend *m_indexer;
    FilterAndBrowseModel *m_searchModel;
    MediaDiscoveryBackend *m_discovery;
};

#endif // MEDIAPLUGIN_H
