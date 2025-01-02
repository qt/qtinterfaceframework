// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFMEDIADEVICEDISCOVERYMODEL_P_H
#define QIFMEDIADEVICEDISCOVERYMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qtifmediaglobal_p.h"
#include "private/qifabstractfeaturelistmodel_p.h"

#include "qifmediadevicediscoverymodel.h"
#include "qifmediadevicediscoverymodelbackendinterface.h"

QT_BEGIN_NAMESPACE

class QIfMediaDeviceDiscoveryModelPrivate : public QIfAbstractFeatureListModelPrivate
{
public:
    QIfMediaDeviceDiscoveryModelPrivate(const QString &interface, QIfMediaDeviceDiscoveryModel *parent);

    void initialize() override;
    void clearToDefaults();
    void resetModel(const QList<QIfServiceObject *> &deviceList);
    void onDeviceAdded(QIfServiceObject *device);
    void onDeviceRemoved(QIfServiceObject *device);

    //TODO rename me to something more handy ?
    QIfMediaDeviceDiscoveryModelBackendInterface *discoveryBackend() const;

    QIfMediaDeviceDiscoveryModel * const q_ptr;

    QList<QIfServiceObject *> m_deviceList;
    Q_DECLARE_PUBLIC(QIfMediaDeviceDiscoveryModel)
};

QT_END_NAMESPACE

#endif // QIFMEDIADEVICEDISCOVERYMODEL_P_H
