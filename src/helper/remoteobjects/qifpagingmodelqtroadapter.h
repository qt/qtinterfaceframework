/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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
