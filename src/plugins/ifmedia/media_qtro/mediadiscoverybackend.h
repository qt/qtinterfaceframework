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

#ifndef MEDIADISCOVERYBACKEND_H
#define MEDIADISCOVERYBACKEND_H

#include <QtIfMedia/QIfMediaDeviceDiscoveryModelBackendInterface>
#include <QtInterfaceFramework/QIfServiceObject>
#include <QIfRemoteObjectsReplicaHelper>
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
};

#endif // MEDIADISCOVERYBACKEND_H
