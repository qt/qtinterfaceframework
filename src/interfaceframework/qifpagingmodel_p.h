// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPAGINGMODEL_P_H
#define QIFPAGINGMODEL_P_H

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

#include <QtInterfaceFramework/private/qifabstractfeaturelistmodel_p.h>
#include <private/qtifglobal_p.h>

#include "qifpagingmodel.h"
#include "qifpagingmodelinterface.h"
#include "qifstandarditem.h"

#include <QtCore/QBitArray>
#include <QtCore/QUuid>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPagingModelPrivate : public QIfAbstractFeatureListModelPrivate
{
public:
    QIfPagingModelPrivate(const QString &interface, QIfPagingModel *model);
    ~QIfPagingModelPrivate() override;

    void initialize() override;
    void onInitializationDone();
    void onCapabilitiesChanged(QUuid identifier, QtInterfaceFrameworkModule::ModelCapabilities capabilities);
    void onDataFetched(QUuid identifier, const QList<QVariant> &items, int start, bool moreAvailable);
    void onCountChanged(QUuid identifier, int new_length);
    void onDataChanged(QUuid identifier, const QList<QVariant> &data, int start, int count);
    void onFetchMoreThresholdReached();
    virtual void resetModel();
    virtual void clearToDefaults();
    const QIfStandardItem *itemAt(int i) const;
    void fetchData(int startIndex);

    QIfPagingModelInterface *backend() const;

    QIfPagingModel * const q_ptr;
    Q_DECLARE_PUBLIC(QIfPagingModel)

    QtInterfaceFrameworkModule::ModelCapabilities m_capabilities;
    int m_chunkSize;

    QList<QVariant> m_itemList;
    QBitArray m_availableChunks;
    bool m_moreAvailable;

    QUuid m_identifier;
    int m_fetchMoreThreshold;
    int m_fetchedDataCount;
    QIfPagingModel::LoadingType m_loadingType;
};

QT_END_NAMESPACE

#endif // QIFPAGINGMODEL_P_H
