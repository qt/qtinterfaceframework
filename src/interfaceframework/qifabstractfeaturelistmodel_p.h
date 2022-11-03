// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFABSTRACTFEATURELISTMODEL_P_H
#define QIFABSTRACTFEATURELISTMODEL_P_H

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

#include <private/qabstractitemmodel_p.h>
#include <private/qifabstractfeature_p.h>
#include <private/qtifglobal_p.h>

#include "qifabstractfeaturelistmodel.h"

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfHelperFeature : public QIfAbstractFeature
{
    Q_OBJECT

public:
    QIfHelperFeature(const QString &interfaceName, QIfAbstractFeatureListModel *model);

    bool acceptServiceObject(QIfServiceObject *so) override;
    bool acceptServiceObjectDefaultImpl(QIfServiceObject *so);
    void connectToServiceObject(QIfServiceObject *so) override;
    void connectToServiceObjectDefaultImpl(QIfServiceObject *so);
    void disconnectFromServiceObject(QIfServiceObject *so) override;
    void disconnectFromServiceObjectDefaultImpl(QIfServiceObject *so);
    void clearServiceObject() override;

    QIfAbstractFeaturePrivate *ifPrivate();
    const QIfAbstractFeaturePrivate *ifPrivate() const;

    using QIfAbstractFeature::interfaceName;
    using QIfAbstractFeature::errorText;
    using QIfAbstractFeature::setError;

    QIfAbstractFeatureListModel *m_model;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractFeatureListModelPrivate : public QAbstractItemModelPrivate
{
public:
    QIfAbstractFeatureListModelPrivate(const QString &interfaceName, QIfAbstractFeatureListModel *model);
    ~QIfAbstractFeatureListModelPrivate() override;

    virtual void initialize();
    QIfFeatureInterface *backend() const;
    template <class T> T backend() const
    {
        return m_feature->ifPrivate()->backend<T>();
    }

    Q_DISABLE_COPY(QIfAbstractFeatureListModelPrivate)

    QIfHelperFeature *m_feature;
};

QT_END_NAMESPACE

#endif // QIFABSTRACTFEATURELISTMODEL_P_H
