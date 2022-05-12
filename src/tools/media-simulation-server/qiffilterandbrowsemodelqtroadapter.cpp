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

#include "qiffilterandbrowsemodelqtroadapter.h"
#include "qifqmlconversion_helper.h"

Q_LOGGING_CATEGORY(qLcROQIfFilterAndBrowseModel, "qt.if.qiffilterandbrowsemodel.remoteobjects", QtInfoMsg)

QIfFilterAndBrowseModelQtRoAdapter::QIfFilterAndBrowseModelQtRoAdapter(QIfFilterAndBrowseModelInterface *parent)
    : QIfFilterAndBrowseModelQtRoAdapter(QStringLiteral("QIfFilterAndBrowseModel"), parent)
{
}

QIfFilterAndBrowseModelQtRoAdapter::QIfFilterAndBrowseModelQtRoAdapter(const QString &remoteObjectsLookupName, QIfFilterAndBrowseModelInterface *parent)
    : QIfFilterAndBrowseModelSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
    , m_helper(this, qLcROQIfFilterAndBrowseModel())
{
    connect(m_backend, &SearchAndBrowseBackend::dataFetched, this, &QIfFilterAndBrowseModelQtRoAdapter::dataFetched);
    connect(m_backend, &SearchAndBrowseBackend::dataChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::dataChanged);
    connect(m_backend, &SearchAndBrowseBackend::countChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::countChanged);
    connect(m_backend, &SearchAndBrowseBackend::canGoBackChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::canGoBackChanged);
    connect(m_backend, &SearchAndBrowseBackend::canGoForwardChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::canGoForwardChanged);
    connect(m_backend, &SearchAndBrowseBackend::supportedCapabilitiesChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::supportedCapabilitiesChanged);
    connect(m_backend, &SearchAndBrowseBackend::availableContentTypesChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::availableContentTypesChanged);
    connect(m_backend, &SearchAndBrowseBackend::contentTypeChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::contentTypeChanged);
    connect(m_backend, &SearchAndBrowseBackend::queryIdentifiersChanged, this, &QIfFilterAndBrowseModelQtRoAdapter::queryIdentifiersChanged);
}

QString QIfFilterAndBrowseModelQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

QStringList QIfFilterAndBrowseModelQtRoAdapter::availableContentTypes() const
{
    return m_backend->property("availableContentTypes").toStringList();
}

void QIfFilterAndBrowseModelQtRoAdapter::setContentType(const QUuid &identifier, const QString &contentType)
{
    m_backend->setContentType(identifier, contentType);
}

void QIfFilterAndBrowseModelQtRoAdapter::setupFilter(const QUuid &identifier, const QVariant &term, const QList<QIfOrderTerm> &orderTerms)
{
    QByteArray data = term.toByteArray();
    QDataStream stream(data);
    QIfAbstractQueryTerm *t = nullptr;
    if (!data.isEmpty())
        stream >> &t;
    m_backend->setupFilter(identifier, t, orderTerms);
}

QVariant QIfFilterAndBrowseModelQtRoAdapter::goBack(const QUuid &identifier)
{
    QIfPendingReplyBase pendingReply = m_backend->goBack(identifier);
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIfFilterAndBrowseModelQtRoAdapter::goForward(const QUuid &identifier, int index)
{
    QIfPendingReplyBase pendingReply = m_backend->goForward(identifier, index);
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

void QIfFilterAndBrowseModelQtRoAdapter::registerInstance(const QUuid &identifier)
{
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    m_backend->registerInstance(identifier);
}

void QIfFilterAndBrowseModelQtRoAdapter::unregisterInstance(const QUuid &identifier)
{
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    m_backend->unregisterInstance(identifier);
}

void QIfFilterAndBrowseModelQtRoAdapter::fetchData(const QUuid &identifier, int start, int count)
{
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    m_backend->fetchData(identifier, start, count);
}

QVariant QIfFilterAndBrowseModelQtRoAdapter::insert(const QUuid &identifier, int index, const QVariant &item)
{
    QIfPendingReplyBase pendingReply = m_backend->insert(identifier, index, item);
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIfFilterAndBrowseModelQtRoAdapter::remove(const QUuid &identifier, int index)
{
    QIfPendingReplyBase pendingReply = m_backend->remove(identifier, index);
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIfFilterAndBrowseModelQtRoAdapter::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    QIfPendingReplyBase pendingReply = m_backend->move(identifier, currentIndex, newIndex);
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}

QVariant QIfFilterAndBrowseModelQtRoAdapter::indexOf(const QUuid &identifier, const QVariant &item)
{
    QIfPendingReplyBase pendingReply = m_backend->indexOf(identifier, item);
    qCDebug(qLcROQIfFilterAndBrowseModel) << Q_FUNC_INFO;
    return m_helper.fromPendingReply(pendingReply);
}
