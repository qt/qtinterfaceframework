/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#ifndef SEARCHBACKEND_H
#define SEARCHBACKEND_H

#include <QtInterfaceFramework/QIfFilterAndBrowseModel>
#include <QtInterfaceFramework/QIfFilterAndBrowseModelInterface>
#include <QtIfMedia/QIfAudioTrackItem>

#include <QSqlDatabase>
#include <QStack>

QT_FORWARD_DECLARE_CLASS(QThreadPool);

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

class SearchAndBrowseBackend : public QIfFilterAndBrowseModelInterface
{
    Q_OBJECT

    Q_PROPERTY(QStringList availableContentTypes READ availableContentTypes CONSTANT)
public:
    explicit SearchAndBrowseBackend(const QSqlDatabase &database, QObject *parent = nullptr);

    QStringList availableContentTypes() const;

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

private slots:
    void search(const QUuid &identifier, const QString &queryString, const QString &type, int start, int count);
    QString createSortOrder(const QString &type, const QList<QIfOrderTerm> &orderTerms);
    QString createWhereClause(const QString &type, QIfAbstractQueryTerm *term);
private:
    QString mapIdentifiers(const QString &type, const QString &identifer);

    QSqlDatabase m_db;
    QThreadPool *m_threadPool;
    QStringList m_contentTypes;
    struct State {
        QString contentType;
        QIfAbstractQueryTerm *queryTerm = nullptr;
        QList<QIfOrderTerm> orderTerms;
        QVariantList items;
    };
    QMap<QUuid, State> m_state;
};

#endif // SEARCHBACKEND_H
