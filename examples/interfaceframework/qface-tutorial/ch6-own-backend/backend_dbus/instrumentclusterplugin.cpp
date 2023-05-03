// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "instrumentclusterplugin.h"

InstrumentClusterPlugin::InstrumentClusterPlugin(QObject *parent)
    : QObject(parent)
    , m_backend(new InstrumentClusterBackend)
{
}

QStringList InstrumentClusterPlugin::interfaces() const
{
    return QStringList(InstrumentClusterModule_InstrumentCluster_iid);
}

QIfFeatureInterface *InstrumentClusterPlugin::interfaceInstance(const QString &interface) const
{
    if (interface == QStringLiteral(InstrumentClusterModule_InstrumentCluster_iid))
        return m_backend;

    return nullptr;
}
