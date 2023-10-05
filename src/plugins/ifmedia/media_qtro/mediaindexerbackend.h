// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef MEDIAINDEXERBACKEND_H
#define MEDIAINDEXERBACKEND_H

#include <QtIfMedia/QIfMediaIndexerControlBackendInterface>
#include <QtIfRemoteObjectsHelper/private/qifremoteobjectsreplicahelper_p.h>
#include <QRemoteObjectNode>

#include "rep_qifmediaindexer_replica.h"

class MediaIndexerBackend : public QIfMediaIndexerControlBackendInterface
{
public:
    explicit MediaIndexerBackend(QObject *parent = nullptr);

public:
    void initialize() override;
    void pause() override;
    void resume() override;

    void updateServiceSettings(const QVariantMap &settings);

protected:
    void setupConnections();
    bool connectToNode();

private:
    QSharedPointer<QIfMediaIndexerReplica> m_replica;
    QRemoteObjectNode *m_node;
    QUrl m_url;
    QIfRemoteObjectsReplicaHelper *m_helper;
    QVariantMap m_serviceSettings;
};

#endif // MEDIAINDEXERBACKEND_H
