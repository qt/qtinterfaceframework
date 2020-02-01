/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "qivimediaindexerqtroadapter.h"

QIviMediaIndexerQtRoAdapter::QIviMediaIndexerQtRoAdapter(MediaIndexerBackend *parent)
    : QIviMediaIndexerQtRoAdapter(QStringLiteral("QtIviMedia.QIviMediaIndexer"), parent)
{
}

QIviMediaIndexerQtRoAdapter::QIviMediaIndexerQtRoAdapter(const QString& remoteObjectsLookupName, MediaIndexerBackend *parent)
    : QIviMediaIndexerSource(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_backend(parent)
{
    connect(m_backend, &MediaIndexerBackend::progressChanged, this, &QIviMediaIndexerQtRoAdapter::progressChanged);
    connect(m_backend, &MediaIndexerBackend::stateChanged, this, &QIviMediaIndexerQtRoAdapter::stateChanged);
}

QString QIviMediaIndexerQtRoAdapter::remoteObjectsLookupName() const
{
    return m_remoteObjectsLookupName;
}

QIviMediaIndexerControl::State QIviMediaIndexerQtRoAdapter::state() const
{
    return m_backend->state();
}

qreal QIviMediaIndexerQtRoAdapter::progress() const
{
    return m_backend->progress();
}

QVariant QIviMediaIndexerQtRoAdapter::pause()
{
    m_backend->pause();
    return QVariant();
}

QVariant QIviMediaIndexerQtRoAdapter::resume()
{
    m_backend->resume();
    return QVariant();
}
