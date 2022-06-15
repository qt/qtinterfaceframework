// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFREMOTEOBJECTSREPLICAHELPER_H
#define QIFREMOTEOBJECTSREPLICAHELPER_H

#include <QtInterfaceFramework/QIfPendingReply>
#include <QtInterfaceFramework/QIfAbstractFeature>
#include <QtRemoteObjects/QRemoteObjectNode>
#include <QtRemoteObjects/QRemoteObjectReplica>
#include <QtRemoteObjects/QRemoteObjectPendingCall>

#include <QtIfRemoteObjectsHelper/qifremoteobjectshelper.h>
#include <QtIfRemoteObjectsHelper/qifremoteobjectspendingresult.h>

QT_BEGIN_NAMESPACE

class QIfRemoteObjectsReplicaHelper : public QObject
{
    Q_OBJECT

public:
    QIfRemoteObjectsReplicaHelper(const QLoggingCategory &category = qtif_private::qLcQtIfRoHelper(), QObject *parent = nullptr);

    QVariant fromRemoteObjectVariant(const QVariant &variant) const;

    template <class T> QIfPendingReply<T> toQIfPendingReply(const QRemoteObjectPendingCall &reply)
    {
        qCDebug(m_category) << "Analyzing QRemoteObjectPendingCall";
        QIfPendingReply<T> ifReply;
        auto watcher = new QRemoteObjectPendingCallWatcher(reply);
        connect(watcher, &QRemoteObjectPendingCallWatcher::finished, watcher, [this, ifReply] (QRemoteObjectPendingCallWatcher *self) mutable {
            if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
                QVariant value = self->returnValue();
#else
                QVariant value = self->returnValue().value<QVariant>();
#endif
                if (value.canConvert<QIfRemoteObjectsPendingResult>()) {
                    auto result = value.value<QIfRemoteObjectsPendingResult>();
                    if (result.failed()) {
                        qCDebug(m_category) << "Pending Result with id:" << result.id() << "failed";
                        ifReply.setFailed();
                    } else {
                        qCDebug(m_category) << "Result not available yet. Waiting for id:" << result.id();
                        m_pendingReplies.insert(result.id(), ifReply);
                    }
                } else {
                    qCDebug(m_category) << "Got the value right away:" << value;
                    ifReply.setSuccess(value);
                }
            } else {
                ifReply.setFailed();
            }
            self->deleteLater();
        });
        return ifReply;
    }

public Q_SLOTS:
    void onPendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value);
    void onReplicaStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState);
    void onNodeError(QRemoteObjectNode::ErrorCode code);

Q_SIGNALS:
    void errorChanged(QIfAbstractFeature::Error error, const QString &message = QString());

private:
    QHash<quint64, QIfPendingReplyBase> m_pendingReplies;
    const QLoggingCategory &m_category;
};

QT_END_NAMESPACE

#endif //QIFREMOTEOBJECTSREPLICAHELPER_H
