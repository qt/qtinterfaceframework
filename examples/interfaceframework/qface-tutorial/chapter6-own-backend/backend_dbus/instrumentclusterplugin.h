// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef INSTRUMENTCLUSTERPLUGIN_H
#define INSTRUMENTCLUSTERPLUGIN_H

#include <QObject>
#include <QIfServiceInterface>
#include "instrumentclusterbackend.h"

class InstrumentClusterPlugin : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "instrumentcluster_dbus.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit InstrumentClusterPlugin(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

private:
    InstrumentClusterBackend *m_backend;
};

#endif // INSTRUMENTCLUSTERPLUGIN_H
