/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef SEARCHANDBROWSEMODEL_H
#define SEARCHANDBROWSEMODEL_H

#include <QtInterfaceFramework/QIfFilterAndBrowseModelInterface>
#include <QtIfMedia/QIfPlayableItem>
#include <QIfRemoteObjectsReplicaHelper>
#include <QRemoteObjectNode>
#include "rep_qiffilterandbrowsemodel_replica.h"

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
    explicit FilterAndBrowseModel(QObject *parent = nullptr, const QString& remoteObjectsLookupName = QStringLiteral("QIfFilterAndBrowseModel"));

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

protected:
    void setupConnections();
    bool connectToNode();

private:
    QString m_remoteObjectsLookupName;
    QSharedPointer<QIfFilterAndBrowseModelReplica> m_replica;
    QRemoteObjectNode *m_node;
    QUrl m_url;
    QIfRemoteObjectsReplicaHelper *m_helper;
};

#endif // SEARCHANDBROWSEMODEL_H
