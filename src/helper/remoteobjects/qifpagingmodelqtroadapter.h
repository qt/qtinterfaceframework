// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPAGINGMODELQTROADAPTER_H
#define QIFPAGINGMODELQTROADAPTER_H

#include <QtInterfaceFramework/QIfPagingModelInterface>
#include "QtIfRemoteObjectsHelper/rep_qifpagingmodel_source.h"

QT_BEGIN_NAMESPACE

template <class ObjectType>
struct QIfPagingModelAddressWrapper: public QIfPagingModelSourceAPI<ObjectType> {
    QIfPagingModelAddressWrapper(ObjectType *object)
        : QIfPagingModelSourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};

class QIfPagingModelQtRoAdapter : public QIfPagingModelSource
{
    Q_OBJECT

public:
    explicit QIfPagingModelQtRoAdapter(const QString &remoteObjectsLookupName, QIfPagingModelInterface *parent = nullptr);

    QString remoteObjectsLookupName() const;

public Q_SLOTS:
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void fetchData(const QUuid &identifier, int start, int count) override;

private:
    QString m_remoteObjectsLookupName;
    QIfPagingModelInterface *m_backend;
};

QT_END_NAMESPACE

#endif // QIFPAGINGMODELQTROADAPTER_H
