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

// This file was generated by qlalr - DO NOT EDIT!

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

#ifndef QTIVIQUERYPARSER_P_H
#define QTIVIQUERYPARSER_P_H

#if defined(ERROR)
#  undef ERROR
#endif

class QtIVIQueryParserTable
{
public:
  enum VariousConstants {
    EOF_SYMBOL = 0,
    AND_OP = 2,
    AND_OP2 = 1,
    ASCENDING = 16,
    BANG = 5,
    DESCENDING = 17,
    EQ_OP = 6,
    EQ_OP2 = 7,
    ERROR = 25,
    FLOATCONSTANT = 21,
    GE_OP = 9,
    GT_OP = 10,
    IC_EQ_OP = 8,
    IDENTIFIER = 22,
    INTCONSTANT = 20,
    LEFT_BRACKET = 18,
    LEFT_PAREN = 14,
    LE_OP = 11,
    LT_OP = 12,
    NE_OP = 13,
    OR_OP = 4,
    OR_OP2 = 3,
    RIGHT_BRACKET = 19,
    RIGHT_PAREN = 15,
    SPACE = 24,
    STRING = 23,

    ACCEPT_STATE = 56,
    RULE_COUNT = 36,
    STATE_COUNT = 57,
    TERMINAL_COUNT = 26,
    NON_TERMINAL_COUNT = 14,

    GOTO_INDEX_OFFSET = 57,
    GOTO_INFO_OFFSET = 114,
    GOTO_CHECK_OFFSET = 114
  };

  static const char  *const    spell [];
  static const short             lhs [];
  static const short             rhs [];
  static const short    goto_default [];
  static const short  action_default [];
  static const short    action_index [];
  static const short     action_info [];
  static const short    action_check [];

  static inline int nt_action (int state, int nt)
  {
    const int yyn = action_index [GOTO_INDEX_OFFSET + state] + nt;
    if (yyn < 0 || action_check [GOTO_CHECK_OFFSET + yyn] != nt)
      return goto_default [nt];

    return action_info [GOTO_INFO_OFFSET + yyn];
  }

  static inline int t_action (int state, int token)
  {
    const int yyn = action_index [state] + token;

    if (yyn < 0 || action_check [yyn] != token)
      return - action_default [state];

    return action_info [yyn];
  }
};


const char *const QtIVIQueryParserTable::spell [] = {
  "end of file", "&", "&&", "|", "||", "!", "==", "=", "~=", ">=", 
  ">", "<=", "<", "!=", "(", ")", "/", "\\", "[", "]", 
  "integer", "float", "identifier", "string", 0, 0};

const short QtIVIQueryParserTable::lhs [] = {
  26, 26, 28, 29, 29, 30, 30, 27, 27, 31, 
  31, 31, 32, 32, 32, 33, 33, 33, 33, 34, 
  34, 34, 34, 36, 36, 35, 35, 35, 35, 35, 
  37, 37, 38, 38, 38, 39};

const short QtIVIQueryParserTable::rhs [] = {
  2, 1, 3, 1, 2, 2, 2, 2, 1, 3, 
  4, 1, 4, 3, 1, 1, 1, 1, 1, 3, 
  3, 3, 3, 1, 1, 1, 1, 1, 1, 1, 
  1, 1, 1, 1, 1, 2};

const short QtIVIQueryParserTable::action_default [] = {
  0, 0, 25, 0, 24, 0, 0, 2, 15, 9, 
  0, 12, 0, 8, 33, 34, 26, 27, 32, 28, 
  29, 35, 30, 0, 0, 20, 21, 0, 0, 0, 
  13, 14, 31, 0, 22, 0, 1, 0, 0, 4, 
  0, 6, 7, 5, 3, 30, 0, 23, 18, 19, 
  16, 17, 0, 0, 10, 11, 36};

const short QtIVIQueryParserTable::goto_default [] = {
  12, 7, 36, 40, 39, 9, 11, 52, 8, 23, 
  10, 24, 22, 0};

const short QtIVIQueryParserTable::action_index [] = {
  33, 19, -26, 88, -26, 68, 13, -13, -26, -26, 
  56, 47, 22, -26, -26, -26, -26, -26, -26, -26, 
  -26, -26, -16, -5, -14, -26, -26, 9, -4, -9, 
  -26, -26, -26, -19, -26, -3, -26, -20, -21, 1, 
  -15, -26, -26, -26, -26, -26, -22, -26, -26, -26, 
  -26, -26, 38, 14, -26, -26, -26, 

  -14, -5, -14, -14, -14, -1, -6, -14, -14, -14, 
  2, -14, -14, -14, -14, -14, -14, -14, -14, -14, 
  -14, -14, -14, 0, -14, -14, -14, -4, -14, -14, 
  -14, -14, -14, -14, -14, -14, -14, -14, -14, 6, 
  -14, -14, -14, -14, -14, -14, -14, -14, -14, -14, 
  -14, -14, -2, -3, -14, -14, -14};

const short QtIVIQueryParserTable::action_info [] = {
  47, 42, 41, 34, 44, 35, 30, -31, 0, 26, 
  0, 31, 0, 37, 38, 4, 2, 37, 38, 14, 
  15, 18, 56, 5, 0, 0, 21, 0, 5, 4, 
  2, 3, 6, 5, 4, 2, 3, 6, 1, 4, 
  2, 3, 6, 53, 0, 0, 0, 5, 49, 48, 
  51, 50, 5, 4, 2, 3, 6, 0, 4, 2, 
  3, 6, 14, 15, 0, 16, 17, 19, 20, 21, 
  0, 0, 0, 27, 0, 0, 0, 0, 0, 0, 
  0, 0, 5, 0, 0, 0, 0, 0, 4, 2, 
  3, 6, 0, 0, 14, 15, 18, 16, 17, 19, 
  20, 21, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 

  13, 29, 55, 54, 28, 33, 32, 0, 0, 43, 
  25, 46, 0, 0, 45, 0, 0, 0, 0, 0};

const short QtIVIQueryParserTable::action_check [] = {
  22, 22, 22, 22, 19, 18, 15, 23, -1, 23, 
  -1, 15, -1, 16, 17, 20, 21, 16, 17, 6, 
  7, 8, 0, 14, -1, -1, 13, -1, 14, 20, 
  21, 22, 23, 14, 20, 21, 22, 23, 5, 20, 
  21, 22, 23, 5, -1, -1, -1, 14, 1, 2, 
  3, 4, 14, 20, 21, 22, 23, -1, 20, 21, 
  22, 23, 6, 7, -1, 9, 10, 11, 12, 13, 
  -1, -1, -1, 5, -1, -1, -1, -1, -1, -1, 
  -1, -1, 14, -1, -1, -1, -1, -1, 20, 21, 
  22, 23, -1, -1, 6, 7, 8, 9, 10, 11, 
  12, 13, -1, -1, -1, -1, -1, -1, -1, -1, 
  -1, -1, -1, -1, 

  5, 5, 5, 5, 5, 11, 12, -1, -1, 3, 
  10, 9, -1, -1, 12, -1, -1, -1, -1, -1};



#include <QtCore>
#include <QtIVICore/QtIVIAbstractQueryTerm>
#include <QtIVICore/private/qtiviqueryterm_p.h>

QT_BEGIN_NAMESPACE

//TODO Find a better way of doing it, this is not reentrant
QString* currentQuery = 0;
unsigned int *currentOffset = 0;
void readQueryBuffer(char *buffer, unsigned int &numBytesRead,int maxBytesToRead)
{
    if (!currentQuery) {
        numBytesRead=0;
        return;
    }

    int numBytesToRead = maxBytesToRead;
    int bytesRemaining = currentQuery->count()-(*currentOffset);
    int i;
    if ( numBytesToRead > bytesRemaining )
        numBytesToRead = bytesRemaining;


    for (i = 0; i < numBytesToRead; i++) {
        buffer[i] = currentQuery->toLatin1().at(*currentOffset+i);
    }

    numBytesRead = numBytesToRead;
    *currentOffset += numBytesToRead;
}

class QtIVIQueryParser: protected QtIVIQueryParserTable
{
public:
    union Value {
      int i;
      float f;
      const QString *s;
    };

public:
    QtIVIQueryParser();
    virtual ~QtIVIQueryParser();

    QtIVIAbstractQueryTerm *parse();

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

    QList<QtIVIOrderTerm> orderTerms() const
    {
        return m_orderList;
    }

protected:
    inline void reallocateStack();

    inline QVariant &sym(int index)
    {
        return sym_stack [tos + index - 1];
    }

    void initBuffer()
    {
        currentQuery = &m_query;
        currentOffset = &m_offset;
    }

    void setErrorString(const QString &error);

    void calcCurrentColumn();

    int nextToken();

    void handleConjunction(bool bangOperator);
    void handleScope(bool bang);

    void negateLeftMostTerm(QtIVIAbstractQueryTerm *term);

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

    QStack<QtIVIAbstractQueryTerm*> m_termStack;
    QStack<QtIVIFilterTerm::Operator> m_operatorStack;
    QStack<QtIVIConjunctionTerm::Conjunction> m_conjunctionStack;
    QList<QtIVIOrderTerm> m_orderList;
};

inline void QtIVIQueryParser::reallocateStack()
{
    int size = state_stack.size();
    if (size == 0)
        size = 128;
    else
        size <<= 1;

    sym_stack.resize(size);
    state_stack.resize(size);
}



QtIVIQueryParser::QtIVIQueryParser():
    m_offset(0),
    column(0),
    tos(0)
{
      reallocateStack();
}

#include "qtiviqueryparser_flex_p.h"

QtIVIQueryParser::~QtIVIQueryParser()
{
    currentOffset = 0;
    currentQuery = 0;

    //We need to reset the lexer to reinitialize it when needed
    yy_init = 0;

    //Get rid of the unused warning
    if (0)
        yyunput(0, 0);}

void QtIVIQueryParser::calcCurrentColumn()
{
    column += yyleng;
}

void QtIVIQueryParser::negateLeftMostTerm(QtIVIAbstractQueryTerm *term)
{
    if (term->type() == QtIVIAbstractQueryTerm::ConjunctionTerm) {
        QtIVIConjunctionTerm* conjunction = static_cast<QtIVIConjunctionTerm*>(term);
        negateLeftMostTerm(conjunction->terms().at(0));
    } else if (term->type() == QtIVIAbstractQueryTerm::ScopeTerm) {
        QtIVIScopeTerm* scopeTerm = static_cast<QtIVIScopeTerm*>(term);
        scopeTerm->d_func()->m_negated = true;
    } else if (term->type() == QtIVIAbstractQueryTerm::FilterTerm) {
        QtIVIFilterTerm* filterTerm = static_cast<QtIVIFilterTerm*>(term);
        filterTerm->d_func()->m_negated = true;
    } else {
        qCritical() << "New Term type added but not handled in" << Q_FUNC_INFO;
    }

    return;
}

void QtIVIQueryParser::handleConjunction(bool bangOperator)
{
    QList<QtIVIAbstractQueryTerm*> list;
    list.prepend(m_termStack.pop());
    list.prepend(m_termStack.pop());

    QtIVIConjunctionTerm *conjunction1 = 0;
    QtIVIConjunctionTerm *conjunction2 = 0;
    int i = 0;
    for (QtIVIAbstractQueryTerm *term : list) {
        if (term->type() == QtIVIAbstractQueryTerm::ConjunctionTerm) {
            QtIVIConjunctionTerm *conj = static_cast<QtIVIConjunctionTerm*>(term);
            if (conj->conjunction() == m_conjunctionStack.top()) {
                if (i == 0)
                    conjunction1 = conj;
                else
                    conjunction2 = conj;
            }
        }
        i++;
    }

    //Handle the bang Operator
    if (bangOperator)
        negateLeftMostTerm(list.at(1));

    QtIVIConjunctionTerm::Conjunction conjunction = m_conjunctionStack.pop();
    //Both are conjunctions, we can sum it together into one.
    if (conjunction1 && conjunction2) {
        conjunction1->d_func()->m_terms += conjunction2->d_func()->m_terms;
        conjunction2->d_func()->m_terms.clear();
        delete conjunction2;
        m_termStack.push(conjunction1);
    } else if (conjunction1) {
        conjunction1->d_func()->m_terms.prepend(list.at(1));
        m_termStack.push(conjunction1);
    } else if (conjunction2) {
        conjunction2->d_func()->m_terms.prepend(list.at(0));
        m_termStack.push(conjunction2);
    } else {
        QtIVIConjunctionTerm *term = new QtIVIConjunctionTerm();
        term->d_func()->m_conjunction = conjunction;
        term->d_func()->m_terms = list;
        m_termStack.push(term);
    }
}

void QtIVIQueryParser::handleScope(bool bangOperator)
{
    QtIVIAbstractQueryTerm *term = m_termStack.pop();

    if (bangOperator)
        negateLeftMostTerm(term);

    QtIVIScopeTerm *scopeTerm = new QtIVIScopeTerm();
    scopeTerm->d_func()->m_term = term;
    m_termStack.push(scopeTerm);
}

bool QtIVIQueryParser::checkIdentifier(const QString &identifer)
{
    if (!m_identifierList.isEmpty() && !m_identifierList.contains(identifer)) {
        QString errorMessage = QString(QLatin1String("Got %1 but expected on of the following identifiers:\n")).arg(identifer);
        for (QString ident : m_identifierList)
            errorMessage.append(QString(QLatin1String("     %1\n")).arg(ident));

        setErrorString(errorMessage);

        qDeleteAll(m_termStack);

        return false;
    }

    return true;
}

QtIVIAbstractQueryTerm *QtIVIQueryParser::parse()
{
    const int INITIAL_STATE = 0;

    int yytoken = -1;

    tos = 0;
    m_offset = 0;
    column = 0;
    state_stack[++tos] = INITIAL_STATE;
    m_termStack.clear();
    m_orderList.clear();

    yyrestart(yyin);

    while (true)
    {
        const int state = state_stack.at(tos);
        if (yytoken == -1 && - TERMINAL_COUNT != action_index [state])
            yytoken = nextToken();

        if (yytoken == ERROR) {
            setErrorString(QString(QLatin1String("Unrecognized token '%1'\n")).arg(QLatin1String(yytext)));
            qDeleteAll(m_termStack);
            return 0;
        }

        if (yytoken == SPACE)
            yytoken = nextToken();

        int act = t_action (state, yytoken);

        if (act == ACCEPT_STATE) {
#ifdef PARSER_DEBUG
            qDebug() << "Representation finished. ToString" << m_termStack.top()->toString();
#endif
            return m_termStack.pop();
        } else if (act > 0) {
            if (++tos == state_stack.size())
                reallocateStack();

            sym_stack [tos] = yylval;
            state_stack [tos] = act;
            yytoken = -1;
        } else if (act < 0) {
            int r = - act - 1;

#ifdef PARSER_DEBUG
            int ridx = rule_index [r];
            qDebug ("*** reduce using rule %d %s :::=", r + 1, spell[rule_info [ridx]]);
            ++ridx;
            for (int i = ridx; i < ridx + rhs [r]; ++i)
            {
                int symbol = rule_info [i];
                if (const char *name = spell [symbol])
                    qDebug (" %s", name);
                else
                    qDebug (" #%d", symbol);
            }
#endif

            tos -= rhs [r];
            act = state_stack.at(tos++);

            switch (r) {

              case 5: {
                    QtIVIOrderTerm order;
                    order.d_func()->m_ascending = true;
                    order.d_func()->m_propertyName = sym(2).toString();
                    m_orderList.append(order);
              } break;

              case 6: {
                    QtIVIOrderTerm order;
                    order.d_func()->m_ascending = false;
                    order.d_func()->m_propertyName = sym(2).toString();
                    m_orderList.append(order);
              } break;

              case 7: {
                QtIVIAbstractQueryTerm *term = m_termStack.top();

                negateLeftMostTerm(term);
              } break;

              case 9: {
                    handleConjunction(false);
              } break;

              case 10: {
                  handleConjunction(true);
              } break;

              case 12: {
                  handleScope(true);
              } break;

              case 13: {
                  handleScope(false);
              } break;

              case 15: {
                  m_conjunctionStack.push(QtIVIConjunctionTerm::Or);
              } break;

              case 16: {
                  m_conjunctionStack.push(QtIVIConjunctionTerm::Or);
              } break;

              case 17: {
                  m_conjunctionStack.push(QtIVIConjunctionTerm::And);
              } break;

              case 18: {
                  m_conjunctionStack.push(QtIVIConjunctionTerm::And);
              } break;

              case 19: {
                    if (!checkIdentifier(sym(1).toString()))
                        return 0;
                    QtIVIFilterTerm *term = new QtIVIFilterTerm();
                    term->d_func()->m_property = sym(1).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(3);
                    m_termStack.push(term);
              } break;

              case 20: {
                    if (!checkIdentifier(sym(1).toString()))
                        return 0;
                    QtIVIFilterTerm *term = new QtIVIFilterTerm();
                    term->d_func()->m_property = sym(1).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(3);
                    m_termStack.push(term);
              } break;

              case 21: {
                    if (!checkIdentifier(sym(3).toString()))
                        return 0;
                    QtIVIFilterTerm *term = new QtIVIFilterTerm();
                    term->d_func()->m_property = sym(3).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(1);
                    m_termStack.push(term);
              } break;

              case 22: {
                    if (!checkIdentifier(sym(3).toString()))
                        return 0;

                    QtIVIFilterTerm::Operator op = m_operatorStack.pop();

                    switch (op) {
                        case QtIVIFilterTerm::GreaterEquals: op = QtIVIFilterTerm::LowerEquals; break;
                        case QtIVIFilterTerm::GreaterThan: op = QtIVIFilterTerm::LowerThan; break;
                        case QtIVIFilterTerm::LowerEquals: op = QtIVIFilterTerm::GreaterEquals; break;
                        case QtIVIFilterTerm::LowerThan: op = QtIVIFilterTerm::GreaterThan; break;
                        default: qFatal("The Grammer was changed but not all logic was ported properly");
                    }

                    QtIVIFilterTerm *term = new QtIVIFilterTerm();
                    term->d_func()->m_property = sym(3).toString();
                    term->d_func()->m_operator = op;
                    term->d_func()->m_value = sym(1);
                    m_termStack.push(term);
              } break;

              case 25: {
                  m_operatorStack.push(QtIVIFilterTerm::GreaterEquals);
              } break;

              case 26: {
                  m_operatorStack.push(QtIVIFilterTerm::GreaterThan);
              } break;

              case 27: {
                  m_operatorStack.push(QtIVIFilterTerm::LowerEquals);
              } break;

              case 28: {
                  m_operatorStack.push(QtIVIFilterTerm::LowerThan);
              } break;

              case 31: {
                  m_operatorStack.push(QtIVIFilterTerm::EqualsCaseInsensitive);
              } break;

              case 32: {
                  m_operatorStack.push(QtIVIFilterTerm::Equals);
              } break;

              case 33: {
                  m_operatorStack.push(QtIVIFilterTerm::Equals);
              } break;

              case 34: {
                  m_operatorStack.push(QtIVIFilterTerm::Unequals);
              } break;

            } // switch

            state_stack [tos] = nt_action (act, lhs [r] - TERMINAL_COUNT);
        } else {
            int ers = state;
            int shifts = 0;
            int reduces = 0;
            QList<int> expectedTokens;
            for (int tk = 0; tk < TERMINAL_COUNT; ++tk) {

                int k = t_action(ers, tk);


                if (! k)
                    continue;
                else if (k < 0)
                    ++reduces;
                else if (spell[tk]) {
                    if (shifts < 7)
                        expectedTokens.append(tk);
                    ++shifts;
                }
            }

            QString errorMessage = QString(QLatin1String("Got %1 but expected on of the following types:\n")).arg(QLatin1String(spell[yytoken]));
            for (int token : expectedTokens)
                errorMessage.append(QString(QLatin1String("     %1\n")).arg(QLatin1String(spell[token])));

            setErrorString(errorMessage);

            qDeleteAll(m_termStack);

            return 0;
        }
    }

    return 0;
}

void QtIVIQueryParser::setErrorString(const QString &error)
{
    int err_col = column - yyleng;

    m_error = QString(QLatin1String(":%1 ERROR: %2")).arg(err_col).arg(error);

    m_error.append(m_query).append(QLatin1String("\n"));
    QString marker(QLatin1String("^"));

    for (int i=0; i<err_col; i++)
        marker.prepend(QLatin1String(" "));

    for (unsigned long i=0; i<yyleng - 1; i++)
        marker.append(QLatin1String("-"));

    m_error.append(marker);
}

QT_END_NAMESPACE


#endif // QTIVIQUERYPARSER_P_H

