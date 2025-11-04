// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifremoteobjectspendingresult_p.h"

QT_BEGIN_NAMESPACE

namespace qtif_private {
    Q_LOGGING_CATEGORY(qLcQtIfRoHelper, "qt.if.remoteobjects.helper", QtInfoMsg)
}

QIfRemoteObjectsPendingResult::QIfRemoteObjectsPendingResult()
    : m_id()
    , m_failed()
{}

QIfRemoteObjectsPendingResult::QIfRemoteObjectsPendingResult(quint64 id, bool failed)
    : m_id(id)
    , m_failed(failed)
{}

quint64 QIfRemoteObjectsPendingResult::id() const
{
    return m_id;
}

void QIfRemoteObjectsPendingResult::setId(quint64 id)
{
    if (id != m_id)
        m_id = id;
}

bool QIfRemoteObjectsPendingResult::failed() const
{
    return m_failed;
}

void QIfRemoteObjectsPendingResult::setFailed(bool failed)
{
    if (failed != m_failed)
        m_failed = failed;
}

QT_END_NAMESPACE

#include "moc_qifremoteobjectspendingresult_p.cpp"
