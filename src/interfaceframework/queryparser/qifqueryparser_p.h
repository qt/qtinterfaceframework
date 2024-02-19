// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QTIFQUERYPARSER_P_H
#define QTIFQUERYPARSER_P_H

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

#include <QtCore>
#include <QtInterfaceFramework/QIfAbstractQueryTerm>
#include <QtInterfaceFramework/private/qifqueryterm_p.h>

#include "qifqueryparsertable_p.h"

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfQueryParser: protected QIfQueryParserTable
{
public:
    union Value {
      int i;
      float f;
      const QString *s;
    };

public:
    Q_DISABLE_COPY_MOVE(QIfQueryParser)

    QIfQueryParser();
    virtual ~QIfQueryParser();

    QIfAbstractQueryTerm *parse();

    void setQuery(const QString& query)
    {
        m_query = query;
    }

    QString lastError() {
        return m_error;
    }

    void setAllowedIdentifiers(const QSet<QString> &list)
    {
        m_identifierList = list;
    }

    QList<QIfOrderTerm> orderTerms() const
    {
        return m_orderList;
    }

protected:
    inline void reallocateStack();

    inline QVariant &sym(int index)
    {
        return sym_stack [tos + index - 1];
    }

    void initBuffer();

    void setErrorString(const QString &error);

    void calcCurrentColumn();

    int nextToken();

    void handleConjunction(bool bangOperator);
    void handleScope(bool bang);

    void negateLeftMostTerm(QIfAbstractQueryTerm *term);

    bool checkIdentifier(const QString &identifer);

protected:
    QString m_query;
    unsigned int m_offset;
    QString m_error;
    QSet<QString> m_identifierList;

    int column;
    int tos;
    QVector<QVariant> sym_stack;
    QVector<int> state_stack;
    QVariant yylval;

    QStack<QIfAbstractQueryTerm*> m_termStack;
    QStack<QIfFilterTerm::Operator> m_operatorStack;
    QStack<QIfConjunctionTerm::Conjunction> m_conjunctionStack;
    QList<QIfOrderTerm> m_orderList;
};

QT_END_NAMESPACE

#endif // QTIFQUERYPARSER_P_H
