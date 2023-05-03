// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifmediaindexerqtroadapter.h"

using namespace Qt::StringLiterals;

QIfMediaIndexerQtRoAdapter::QIfMediaIndexerQtRoAdapter(MediaIndexerBackend *parent)
    : QIfMediaIndexerQtRoAdapter(u"QtIfMedia.QIfMediaIndexer"_s, parent)
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
