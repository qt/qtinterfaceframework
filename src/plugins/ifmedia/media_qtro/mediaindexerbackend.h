/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
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

#ifndef MEDIAINDEXERBACKEND_H
#define MEDIAINDEXERBACKEND_H

#include <QtIfMedia/QIfMediaIndexerControlBackendInterface>
#include <QIfRemoteObjectsReplicaHelper>
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

protected:
    void setupConnections();
    bool connectToNode();

private:
    QSharedPointer<QIfMediaIndexerReplica> m_replica;
    QRemoteObjectNode *m_node;
    QUrl m_url;
    QIfRemoteObjectsReplicaHelper *m_helper;
};

#endif // MEDIAINDEXERBACKEND_H
