// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef SEARCHBACKEND_H
#define SEARCHBACKEND_H

#include <QtInterfaceFramework/QIfFilterAndBrowseModel>
#include <QtInterfaceFramework/QIfFilterAndBrowseModelInterface>
#include <QtIfMedia/QIfAmFmTunerStation>

class AmFmTunerBackend;

class SearchAndBrowseBackend : public QIfFilterAndBrowseModelInterface
{
    Q_OBJECT
public:
    explicit SearchAndBrowseBackend(AmFmTunerBackend *tunerBackend, QObject *parent = nullptr);

    void initialize() override;
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms) override;
    void fetchData(const QUuid &identifier, int start, int count) override;
    QIfPendingReply<QString> goBack(const QUuid &identifier) override;
    QIfPendingReply<QString> goForward(const QUuid &identifier, int index) override;

    QIfPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &item) override;
    QIfPendingReply<void> remove(const QUuid &identifier, int index) override;
    QIfPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) override;
    QIfPendingReply<int> indexOf(const QUuid &identifier, const QVariant &item) override;
private:

    AmFmTunerBackend *m_tunerBackend;
    QVector<QIfAmFmTunerStation> m_presets;
    QHash<QUuid, QString> m_contentType;
};

#endif // SEARCHBACKEND_H
