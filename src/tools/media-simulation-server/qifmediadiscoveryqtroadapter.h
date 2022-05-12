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
