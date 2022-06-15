// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFMEDIADISCOVERYMODELQTROADAPTER_H
#define QIFMEDIADISCOVERYMODELQTROADAPTER_H

#include "mediadiscoverybackend.h"
#include "rep_qifmediadiscoverymodel_source.h"

#include <QRemoteObjectRegistryHost>

template <class ObjectType>
struct QIfMediaDiscoveryModelAddressWrapper: public QIfMediaDiscoveryModelSourceAPI<ObjectType> {
    QIfMediaDiscoveryModelAddressWrapper(ObjectType *object)
        : QIfMediaDiscoveryModelSourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};

class QIfMediaDiscoveryModelQtRoAdapter : public QIfMediaDiscoveryModelSource
{
public:
    QIfMediaDiscoveryModelQtRoAdapter(MediaDiscoveryBackend *parent);
    QIfMediaDiscoveryModelQtRoAdapter(const QString& remoteObjectsLookupName, MediaDiscoveryBackend *parent);

    QString remoteObjectsLookupName() const;

public Q_SLOTS:
    QStringList devices() const override;
    void onDeviceAdded(QIfServiceObject *device);
    void onDeviceRemoved(QIfServiceObject *device);

private:
    void createDeviceAdapter(QIfMediaDevice *device);

    QString m_remoteObjectsLookupName;
    MediaDiscoveryBackend *m_backend;
    QHash<QString, QObject*> m_hostMap;
};

#endif // QIFMEDIAINDEXERQTROADAPTER_H
