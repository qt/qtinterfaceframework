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

#include "qifremoteobjectspendingresult.h"

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
