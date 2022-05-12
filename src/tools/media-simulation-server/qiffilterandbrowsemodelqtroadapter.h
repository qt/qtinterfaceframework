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

#ifndef QIFSEARCHANDBROWSEMODELQTROADAPTER_H
#define QIFSEARCHANDBROWSEMODELQTROADAPTER_H

#include <QIfRemoteObjectsSourceHelper>

#include "searchandbrowsebackend.h"
#include "rep_qiffilterandbrowsemodel_source.h"

template <class ObjectType>
struct QIfFilterAndBrowseModelAddressWrapper: public QIfFilterAndBrowseModelSourceAPI<ObjectType> {
    QIfFilterAndBrowseModelAddressWrapper(ObjectType *object)
        : QIfFilterAndBrowseModelSourceAPI<ObjectType>(object, object->remoteObjectsLookupName())
    {}
};

class QIfFilterAndBrowseModelQtRoAdapter : public QIfFilterAndBrowseModelSource
{
public:
    QIfFilterAndBrowseModelQtRoAdapter(QIfFilterAndBrowseModelInterface *parent);
    QIfFilterAndBrowseModelQtRoAdapter(const QString& remoteObjectsLookupName, QIfFilterAndBrowseModelInterface *parent);

    QString remoteObjectsLookupName() const;
    QStringList availableContentTypes() const override;

public Q_SLOTS:
    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, const QVariant &term, const QList<QIfOrderTerm> &orderTerms) override;
    QVariant goBack(const QUuid &identifier) override;
    QVariant goForward(const QUuid &identifier, int index) override;
    QVariant insert(const QUuid &identifier, int index, const QVariant &item) override;
    QVariant remove(const QUuid &identifier, int index) override;
    QVariant move(const QUuid &identifier, int currentIndex, int newIndex) override;
    QVariant indexOf(const QUuid &identifier, const QVariant &item) override;

    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void fetchData(const QUuid &identifier, int start, int count) override;

private:
    QString m_remoteObjectsLookupName;
    QIfFilterAndBrowseModelInterface *m_backend;
    QIfRemoteObjectsSourceHelper<QIfFilterAndBrowseModelQtRoAdapter> m_helper;
};

#endif // QIFSEARCHANDBROWSEMODELQTROADAPTER_H
