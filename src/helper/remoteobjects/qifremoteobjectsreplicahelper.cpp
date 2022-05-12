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

#include "qifremoteobjectsreplicahelper.h"

QT_BEGIN_NAMESPACE

QIfRemoteObjectsReplicaHelper::QIfRemoteObjectsReplicaHelper(const QLoggingCategory &category, QObject *parent)
    : QObject(parent)
    , m_category(category)
{
    qRegisterMetaType<QIfRemoteObjectsPendingResult>();
}

QVariant QIfRemoteObjectsReplicaHelper::fromRemoteObjectVariant(const QVariant &variant) const
{
    return variant.value<QVariant>();
}

void QIfRemoteObjectsReplicaHelper::onPendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value)
{
    qCDebug(m_category) << "pending result available for id:" << id;
    if (!m_pendingReplies.contains(id)) {
        qCDebug(m_category) << "Received a result for an unexpected id:" << id << ". Ignoring!";
        return;
    }

    QIfPendingReplyBase ifReply = m_pendingReplies.take(id);

    if (isSuccess)
        ifReply.setSuccess(value);
    else
        ifReply.setFailed();
}

void QIfRemoteObjectsReplicaHelper::onReplicaStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState)
{
    Q_UNUSED(oldState)

    if (newState == QRemoteObjectReplica::Suspect) {
        qCWarning(m_category) << "QRemoteObjectReplica error, connection to the source lost";
        emit errorChanged(QIfAbstractFeature::Unknown,
                          QStringLiteral("QRemoteObjectReplica error, connection to the source lost"));
    } else if (newState == QRemoteObjectReplica::SignatureMismatch) {
        qCWarning(m_category) << "QRemoteObjectReplica error, signature mismatch";
        emit errorChanged(QIfAbstractFeature::Unknown,
                          QStringLiteral("QRemoteObjectReplica error, signature mismatch"));
    } else if (newState == QRemoteObjectReplica::Valid) {
        emit errorChanged(QIfAbstractFeature::NoError);
    }
}

void QIfRemoteObjectsReplicaHelper::onNodeError(QRemoteObjectNode::ErrorCode code)
{
    qCWarning(m_category) << "QRemoteObjectNode error, code: " << code;
    QMetaEnum metaEnum = QMetaEnum::fromType<QRemoteObjectNode::ErrorCode>();
    emit errorChanged(QIfAbstractFeature::Unknown, QStringLiteral("QRemoteObjectNode error, code: ") + QLatin1String(metaEnum.valueToKey(code)));
}

QT_END_NAMESPACE
