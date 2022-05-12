/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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
