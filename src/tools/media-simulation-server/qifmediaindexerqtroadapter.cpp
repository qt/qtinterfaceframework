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

#include "qifmediaindexerqtroadapter.h"

QIfMediaIndexerQtRoAdapter::QIfMediaIndexerQtRoAdapter(MediaIndexerBackend *parent)
    : QIfMediaIndexerQtRoAdapter(QStringLiteral("QtIfMedia.QIfMediaIndexer"), parent)
{
}

QIfMediaIndexerQtRoAdapter::QIfMediaIndexerQtRoAdapter(const QString& remoteObjectsLookupName, MediaIndexerBackend *parent)
    : QIfMediaIndexerSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
{
    connect(m_backend, &MediaIndexerBackend::progressChanged, this, &QIfMediaIndexerQtRoAdapter::progressChanged);
    connect(m_backend, &MediaIndexerBackend::stateChanged, this, &QIfMediaIndexerQtRoAdapter::stateChanged);
}

QString QIfMediaIndexerQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

QIfMediaIndexerControl::State QIfMediaIndexerQtRoAdapter::state() const
{
    return m_backend->state();
}

qreal QIfMediaIndexerQtRoAdapter::progress() const
{
    return m_backend->progress();
}

QVariant QIfMediaIndexerQtRoAdapter::pause()
{
    m_backend->pause();
    return QVariant();
}

QVariant QIfMediaIndexerQtRoAdapter::resume()
{
    m_backend->resume();
    return QVariant();
}
