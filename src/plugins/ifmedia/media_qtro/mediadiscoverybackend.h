// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef MEDIADISCOVERYBACKEND_H
#define MEDIADISCOVERYBACKEND_H

#include <QtIfMedia/QIfMediaDeviceDiscoveryModelBackendInterface>
#include <QtInterfaceFramework/QIfServiceObject>
#include <QtIfRemoteObjectsHelper/private/qifremoteobjectsreplicahelper_p.h>
#include <QRemoteObjectNode>

#include "rep_qifmediadiscoverymodel_replica.h"


class MediaDiscoveryBackend : public QIfMediaDeviceDiscoveryModelBackendInterface
{
    Q_OBJECT

public:
    explicit MediaDiscoveryBackend(QObject *parent = nullptr);

    void initialize() override;

public Q_SLOTS:
    void onDevicesChanged(const QStringList &devices);
    void onDeviceAdded(const QString &device);
    void onDeviceRemoved(const QString &device);

    void updateServiceSettings(const QVariantMap &settings);

protected:
    void setupConnections();
    bool connectToNode();

private:
    QSharedPointer<QIfMediaDiscoveryModelReplica> m_replica;
    QRemoteObjectNode *m_node;
    QUrl m_url;
    QHash<QString, QIfServiceObject*> m_deviceMap;
    bool m_initialized;
    QIfRemoteObjectsReplicaHelper *m_helper;
    QVariantMap m_serviceSettings;
};

#endif // MEDIADISCOVERYBACKEND_H
