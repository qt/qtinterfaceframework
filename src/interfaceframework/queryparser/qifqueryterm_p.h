// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFQUERYTERM_P_H
#define QIFQUERYTERM_P_H

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

#include "qifqueryterm.h"
#include "private/qglobal_p.h"

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfConjunctionTermPrivate
{
public:
    QIfConjunctionTermPrivate();

    QList<QIfAbstractQueryTerm*> m_terms;
    QIfConjunctionTerm::Conjunction m_conjunction;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfScopeTermPrivate
{
public:
    QIfScopeTermPrivate();

    QIfAbstractQueryTerm *m_term;
    bool m_negated;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfFilterTermPrivate
{
public:
    QIfFilterTermPrivate();

    QString operatorToString() const;

    QString m_property;
    QIfFilterTerm::Operator m_operator;
    QVariant m_value;
    bool m_negated;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfOrderTermPrivate : public QSharedData
{
public:
    QIfOrderTermPrivate();
    QIfOrderTermPrivate(const QIfOrderTermPrivate &other);

    bool m_ascending;
    QString m_propertyName;
};

QT_END_NAMESPACE

#endif // QIFQUERYTERM_P_H
