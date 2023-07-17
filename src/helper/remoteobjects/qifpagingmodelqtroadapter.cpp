// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifpagingmodelqtroadapter.h"

QIfPagingModelQtRoAdapter::QIfPagingModelQtRoAdapter(const QString &remoteObjectsLookupName, QIfPagingModelInterface *parent)
    : QIfPagingModelSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
{
    connect(m_backend, &QIfPagingModelInterface::supportedCapabilitiesChanged, this, &QIfPagingModelQtRoAdapter::supportedCapabilitiesChanged);
    connect(m_backend, &QIfPagingModelInterface::countChanged, this, &QIfPagingModelQtRoAdapter::countChanged);
    connect(m_backend, &QIfPagingModelInterface::dataFetched, this, &QIfPagingModelQtRoAdapter::dataFetched);
    connect(m_backend, &QIfPagingModelInterface::dataChanged, this, &QIfPagingModelQtRoAdapter::dataChanged);
}

QString QIfPagingModelQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

void QIfPagingModelQtRoAdapter::registerInstance(const QUuid &identifier)
{
    m_backend->registerInstance(identifier);
}

void QIfPagingModelQtRoAdapter::unregisterInstance(const QUuid &identifier)
{
    m_backend->unregisterInstance(identifier);
}

void QIfPagingModelQtRoAdapter::fetchData(const QUuid &identifier, int start, int count)
{
    m_backend->fetchData(identifier, start, count);
}

#include "moc_qifpagingmodelqtroadapter.cpp"
