/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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
