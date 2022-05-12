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

#ifndef QIFREMOTEOBJECTSSOURCEHELPER_H
#define QIFREMOTEOBJECTSSOURCEHELPER_H

#include <QtCore/QObject>
#include <QtInterfaceFramework/QIfPendingReply>

#include <QtIfRemoteObjectsHelper/qifremoteobjectshelper.h>
#include <QtIfRemoteObjectsHelper/qifremoteobjectspendingresult.h>

QT_BEGIN_NAMESPACE

template <class T> class QIfRemoteObjectsSourceHelper
{
public:
    QIfRemoteObjectsSourceHelper(T *adapter, const QLoggingCategory &category = qtif_private::qLcQtIfRoHelper())
        : m_adapter(adapter)
        , m_replyCounter(0)
        , m_category(category)
    {
        qRegisterMetaType<QIfRemoteObjectsPendingResult>();
    }

    QVariant toRemoteObjectVariant(const QVariant &variant) const
    {
        // QtRemoteObjects always uses QVariant::data() and passes it directly to qt_metacall. This doesn't
        // work for methods/signals which take a QVariant.
        // The workaround is put the QVariant in a QVariant as a container.
        // QTBUG-75056
        return QVariant(QMetaType(QMetaType::QVariant), &variant);
    }

    QVariant fromPendingReply(const QIfPendingReplyBase &pendingReply)
    {
        if (pendingReply.isSuccessful()) {
            qCDebug(m_category) << "Returning result right away";
            return pendingReply.value();
        } else { //reply not yet ready or failed
            const quint64 id = ++m_replyCounter;
            if (pendingReply.isResultAvailable()) { // the call failed
                qCDebug(m_category) << "Returning failed reply";
                return QVariant::fromValue(QIfRemoteObjectsPendingResult(id, true /* failed */));
            }
            QIfRemoteObjectsPendingResult result = QIfRemoteObjectsPendingResult(id, false /* failed */);
            qCDebug(m_category) << "Returning a pending result: id:" << id;
            QObject::connect(pendingReply.watcher(), &QIfPendingReplyWatcher::valueChanged, [this, pendingReply, id] (const QVariant &value) {
                qCDebug(m_category) << "Value for pending result available: id:" << id << "value:" << value;
                Q_EMIT m_adapter->pendingResultAvailable(id, pendingReply.isSuccessful(), value);
            });
            return QVariant::fromValue(result);
        }
    }

private:
    T *m_adapter;
    quint64 m_replyCounter;
    const QLoggingCategory &m_category;
};

QT_END_NAMESPACE

#endif //QIFREMOTEOBJECTSSOURCEHELPER_H
