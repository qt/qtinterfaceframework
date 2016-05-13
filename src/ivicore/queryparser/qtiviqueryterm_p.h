/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#ifndef QTIVIQUERYTERM_P_H
#define QTIVIQUERYTERM_P_H

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

#include "qtiviqueryterm.h"

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QtIVIConjunctionTermPrivate
{
public:
    QtIVIConjunctionTermPrivate();

    QList<QtIVIAbstractQueryTerm*> m_terms;
    QtIVIConjunctionTerm::Conjunction m_conjunction;
};

class Q_QTIVICORE_EXPORT QtIVIScopeTermPrivate
{
public:
    QtIVIScopeTermPrivate();

    QtIVIAbstractQueryTerm* m_term;
    bool m_negated;
};

class Q_QTIVICORE_EXPORT QtIVIFilterTermPrivate
{
public:
    QtIVIFilterTermPrivate();

    QString operatorToString() const;

    QString m_property;
    QtIVIFilterTerm::Operator m_operator;
    QVariant m_value;
    bool m_negated;
};

class Q_QTIVICORE_EXPORT QtIVIOrderTermPrivate
{
public:
    QtIVIOrderTermPrivate();

    bool m_ascending;
    QString m_propertyName;
};

QT_END_NAMESPACE

#endif // QTIVIQUERYTERM_P_H
