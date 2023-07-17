// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifremoteobjectsreplicahelper.h"

using namespace Qt::StringLiterals;

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
                          u"QRemoteObjectReplica error, connection to the source lost"_s);
    } else if (newState == QRemoteObjectReplica::SignatureMismatch) {
        qCWarning(m_category) << "QRemoteObjectReplica error, signature mismatch";
        emit errorChanged(QIfAbstractFeature::Unknown,
                          u"QRemoteObjectReplica error, signature mismatch"_s);
    } else if (newState == QRemoteObjectReplica::Valid) {
        emit errorChanged(QIfAbstractFeature::NoError);
    }
}

void QIfRemoteObjectsReplicaHelper::onNodeError(QRemoteObjectNode::ErrorCode code)
{
    qCWarning(m_category) << "QRemoteObjectNode error, code: " << code;
    QMetaEnum metaEnum = QMetaEnum::fromType<QRemoteObjectNode::ErrorCode>();
    emit errorChanged(QIfAbstractFeature::Unknown, u"QRemoteObjectNode error, code: "_s + QLatin1String(metaEnum.valueToKey(code)));
}

QT_END_NAMESPACE

#include "moc_qifremoteobjectsreplicahelper.cpp"
