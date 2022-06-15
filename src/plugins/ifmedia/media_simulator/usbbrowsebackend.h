// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef USBBROWSEBACKEND_H
#define USBBROWSEBACKEND_H

#include "searchandbrowsebackend.h"

class UsbBrowseBackend : public QIfFilterAndBrowseModelInterface
{
    Q_OBJECT

    Q_PROPERTY(QStringList availableContentTypes READ availableContentTypes CONSTANT)
public:
    UsbBrowseBackend(const QString &path, QObject *parent = nullptr);

    QStringList availableContentTypes() const;

    void initialize() override;
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms) override;
    void fetchData(const QUuid &identifier, int start, int count) override;
//    bool canGoBack(const QUuid &identifier, const QString &type) override;
    QIfPendingReply<QString> goBack(const QUuid &identifier) override;
//    bool canGoForward(const QUuid &identifier, const QString &type, const QString &itemId) override;
    QIfPendingReply<QString> goForward(const QUuid &identifier, int index) override;

    QIfPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &item) override;
    QIfPendingReply<void> remove(const QUuid &identifier, int index) override;
    QIfPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) override;
    QIfPendingReply<int> indexOf(const QUuid &identifier, const QVariant &item) override;

private:
    QString m_rootFolder;
    struct State {
        QString contentType;
        QVariantList items;
    };
    QMap<QUuid, State> m_state;
};

#endif // USBBROWSEBACKEND_H
