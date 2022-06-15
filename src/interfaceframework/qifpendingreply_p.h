// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPENDINGREPLY_P_H
#define QIFPENDINGREPLY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qobject_p.h>
#include <private/qtifglobal_p.h>

#include "qifpendingreply.h"

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPendingReplyWatcherPrivate : public QObjectPrivate
{
public:
    QIfPendingReplyWatcherPrivate(int userType, QIfPendingReplyWatcher *parent);

    void setSuccess(const QVariant &value);
    void callSuccessCallback();
    void callFailedCallback();

    QIfPendingReplyWatcher * const q_ptr;
    Q_DECLARE_PUBLIC(QIfPendingReplyWatcher)
    Q_DISABLE_COPY(QIfPendingReplyWatcherPrivate)

    int m_type;
    bool m_resultAvailable;
    bool m_success;
    QVariant m_data;
    QJSValue m_successFunctor;
    QJSValue m_failedFunctor;
    QJSEngine *m_callbackEngine;
};

QT_END_NAMESPACE

#endif // QIFPENDINGREPLY_P_H
