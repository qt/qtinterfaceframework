// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSEARCHMODEL_P_H
#define QIFSEARCHMODEL_P_H

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

#include <QtInterfaceFramework/private/qifpagingmodel_p.h>
#include <private/qtifglobal_p.h>

#include "qifqueryterm.h"
#include "qiffilterandbrowsemodel.h"
#include "qiffilterandbrowsemodelinterface.h"
#include "qifstandarditem.h"

#include <QBitArray>
#include <QUuid>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfFilterAndBrowseModelPrivate : public QIfPagingModelPrivate
{
public:
    QIfFilterAndBrowseModelPrivate(const QString &interface, QIfFilterAndBrowseModel *model);
    ~QIfFilterAndBrowseModelPrivate() override;

    void resetModel() override;
    void parseQuery();
    void setupFilter(QIfAbstractQueryTerm* queryTerm, const QList<QIfOrderTerm> &orderTerms);
    void clearToDefaults() override;
    void onCanGoForwardChanged(const QUuid &identifier, const QVector<bool> &indexes, int start);
    void onCanGoBackChanged(const QUuid &identifier, bool canGoBack);
    void onContentTypeChanged(const QUuid &identifier, const QString &contentType);
    void onAvailableContentTypesChanged(const QStringList &contentTypes);
    void onQueryIdentifiersChanged(const QUuid &identifier, const QSet<QString> &queryIdentifiers);

    QIfFilterAndBrowseModelInterface *searchBackend() const;
    void updateContentType(const QString &contentType);

    QIfFilterAndBrowseModel * const q_ptr;
    Q_DECLARE_PUBLIC(QIfFilterAndBrowseModel)

    QString m_query;

    QIfAbstractQueryTerm *m_queryTerm;
    QList<QIfOrderTerm> m_orderTerms;

    QString m_contentTypeRequested;
    QString m_contentType;
    QStringList m_availableContentTypes;
    QSet<QString> m_queryIdentifiers;
    QVector<bool> m_canGoForward;
    bool m_canGoBack;
};

QT_END_NAMESPACE

#endif // QIFSEARCHMODEL_P_H
