// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef SEARCHANDBROWSEMODEL_H
#define SEARCHANDBROWSEMODEL_H

#include <QtInterfaceFramework/QIfFilterAndBrowseModelInterface>
#include <QtIfMedia/QIfPlayableItem>
#include <QIfRemoteObjectsReplicaHelper>
#include <QRemoteObjectNode>
#include "rep_qiffilterandbrowsemodel_replica.h"

using namespace Qt::StringLiterals;

class SearchAndBrowseItem : public QIfPlayableItem
{
    Q_GADGET

public:
    QString id() const override { return m_name; }
    QString name() const override { return m_name; }
    void setName(const QString &name) { m_name = name; }
    QString type() const override { return m_type; }
    void setType(const QString &type) { m_type = type; }

private:
    QString m_name;
    QString m_type;
};
Q_DECLARE_METATYPE(SearchAndBrowseItem)

QDataStream &operator<<(QDataStream &stream, const SearchAndBrowseItem &obj);
QDataStream &operator>>(QDataStream &stream, SearchAndBrowseItem &obj);

class FilterAndBrowseModel : public QIfFilterAndBrowseModelInterface
{
    Q_OBJECT
public:
    explicit FilterAndBrowseModel(QObject *parent = nullptr, const QString& remoteObjectsLookupName = u"QIfFilterAndBrowseModel"_s);

    void initialize() override;
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;
    void fetchData(const QUuid &identifier, int start, int count) override;

    void setContentType(const QUuid &identifier, const QString &contentType) override;
    void setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms) override;
    QIfPendingReply<QString> goBack(const QUuid &identifier) override;
    QIfPendingReply<QString> goForward(const QUuid &identifier, int index) override;
    QIfPendingReply<void> insert(const QUuid &identifier, int index, const QVariant &item) override;
    QIfPendingReply<void> remove(const QUuid &identifier, int index) override;
    QIfPendingReply<void> move(const QUuid &identifier, int currentIndex, int newIndex) override;
    QIfPendingReply<int> indexOf(const QUuid &identifier, const QVariant &item) override;

    void updateServiceSettings(const QVariantMap &settings);

protected:
    void setupConnections();
    bool connectToNode();

private:
    QString m_remoteObjectsLookupName;
    QSharedPointer<QIfFilterAndBrowseModelReplica> m_replica;
    QRemoteObjectNode *m_node;
    QUrl m_url;
    QIfRemoteObjectsReplicaHelper *m_helper;
    QVariantMap m_serviceSettings;
};

#endif // SEARCHANDBROWSEMODEL_H
