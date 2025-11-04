// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifqueryparser_p.h"

//This is needed to comparison warning in the generated flex code.
#define YY_TYPEDEF_YY_SIZE_T
typedef int yy_size_t;

#include "qifqueryparser_flex_p.h"

QString* currentQuery = nullptr;
unsigned int *currentOffset = nullptr;

void readQueryBuffer(char *buffer, unsigned int &numBytesRead,int maxBytesToRead)
{
    if (!currentQuery) {
        numBytesRead=0;
        return;
    }

    int numBytesToRead = maxBytesToRead;
    int bytesRemaining = currentQuery->size()-(*currentOffset);
    int i;
    if ( numBytesToRead > bytesRemaining )
        numBytesToRead = bytesRemaining;


    for (i = 0; i < numBytesToRead; i++) {
        buffer[i] = currentQuery->toLatin1().at(*currentOffset+i);
    }

    numBytesRead = numBytesToRead;
    *currentOffset += numBytesToRead;
}

QT_BEGIN_NAMESPACE

QIfQueryParser::QIfQueryParser():
    m_offset(0),
    column(0),
    tos(0)
{
      reallocateStack();
}

inline void QIfQueryParser::reallocateStack()
{
      int size = state_stack.size();
      if (size == 0)
        size = 128;
      else
        size <<= 1;

      sym_stack.resize(size);
      state_stack.resize(size);
}

QIfQueryParser::~QIfQueryParser()
{
    currentOffset = nullptr;
    currentQuery = nullptr;

    //We need to reset the lexer to reinitialize it when needed
    yy_init = 0;

    //Get rid of the unused warning
    if (0)
        yyunput(0, 0);
}

void QIfQueryParser::initBuffer()
{
    currentQuery = &m_query;
    currentOffset = &m_offset;
}

void QIfQueryParser::calcCurrentColumn()
{
    column += yyleng;
}

void QIfQueryParser::negateLeftMostTerm(QIfAbstractQueryTerm *term)
{
    if (term->type() == QIfAbstractQueryTerm::ConjunctionTerm) {
        QIfConjunctionTerm* conjunction = static_cast<QIfConjunctionTerm*>(term);
        negateLeftMostTerm(conjunction->terms().at(0));
    } else if (term->type() == QIfAbstractQueryTerm::ScopeTerm) {
        QIfScopeTerm* scopeTerm = static_cast<QIfScopeTerm*>(term);
        scopeTerm->d_func()->m_negated = true;
    } else if (term->type() == QIfAbstractQueryTerm::FilterTerm) {
        QIfFilterTerm* filterTerm = static_cast<QIfFilterTerm*>(term);
        filterTerm->d_func()->m_negated = true;
    } else {
        qCritical() << "New Term type added but not handled in" << Q_FUNC_INFO;
    }

    return;
}

void QIfQueryParser::handleConjunction(bool bangOperator)
{
    QList<QIfAbstractQueryTerm*> list;
    list.prepend(m_termStack.pop());
    list.prepend(m_termStack.pop());

    QIfConjunctionTerm *conjunction1 = nullptr;
    QIfConjunctionTerm *conjunction2 = nullptr;
    int i = 0;
    for (QIfAbstractQueryTerm *term : list) {
        if (term->type() == QIfAbstractQueryTerm::ConjunctionTerm) {
            QIfConjunctionTerm *conj = static_cast<QIfConjunctionTerm*>(term);
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

    QIfConjunctionTerm::Conjunction conjunction = m_conjunctionStack.pop();
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
        QIfConjunctionTerm *term = new QIfConjunctionTerm();
        term->d_func()->m_conjunction = conjunction;
        term->d_func()->m_terms = list;
        m_termStack.push(term);
    }
}

void QIfQueryParser::handleScope(bool bangOperator)
{
    QIfAbstractQueryTerm *term = m_termStack.pop();

    if (bangOperator)
        negateLeftMostTerm(term);

    QIfScopeTerm *scopeTerm = new QIfScopeTerm();
    scopeTerm->d_func()->m_term = term;
    m_termStack.push(scopeTerm);
}

bool QIfQueryParser::checkIdentifier(const QString &identifer)
{
    if (!m_identifierList.isEmpty() && !m_identifierList.contains(identifer)) {
        QString errorMessage = QString(QLatin1String("Got %1 but expected on of the following identifiers:\n")).arg(identifer);
        for (const QString &ident : std::as_const(m_identifierList))
            errorMessage.append(QString(QLatin1String("     %1\n")).arg(ident));

        setErrorString(errorMessage);

        qDeleteAll(m_termStack);

        return false;
    }

    return true;
}

QIfAbstractQueryTerm *QIfQueryParser::parse()
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
                    QIfOrderTerm order;
                    order.d->m_ascending = true;
                    order.d->m_propertyName = sym(2).toString();
                    m_orderList.append(order);
              } break;

              case 6: {
                    QIfOrderTerm order;
                    order.d->m_ascending = false;
                    order.d->m_propertyName = sym(2).toString();
                    m_orderList.append(order);
              } break;

              case 7: {
                QIfAbstractQueryTerm *term = m_termStack.top();

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
                  m_conjunctionStack.push(QIfConjunctionTerm::Or);
              } break;

              case 16: {
                  m_conjunctionStack.push(QIfConjunctionTerm::Or);
              } break;

              case 17: {
                  m_conjunctionStack.push(QIfConjunctionTerm::And);
              } break;

              case 18: {
                  m_conjunctionStack.push(QIfConjunctionTerm::And);
              } break;

              case 19: {
                    if (!checkIdentifier(sym(1).toString()))
                        return 0;
                    QIfFilterTerm *term = new QIfFilterTerm();
                    term->d_func()->m_property = sym(1).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(3);
                    m_termStack.push(term);
              } break;

              case 20: {
                    if (!checkIdentifier(sym(1).toString()))
                        return 0;
                    QIfFilterTerm *term = new QIfFilterTerm();
                    term->d_func()->m_property = sym(1).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(3);
                    m_termStack.push(term);
              } break;

              case 21: {
                    if (!checkIdentifier(sym(3).toString()))
                        return 0;
                    QIfFilterTerm *term = new QIfFilterTerm();
                    term->d_func()->m_property = sym(3).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(1);
                    m_termStack.push(term);
              } break;

              case 22: {
                    if (!checkIdentifier(sym(3).toString()))
                        return 0;

                    QIfFilterTerm::Operator op = m_operatorStack.pop();

                    switch (op) {
                        case QIfFilterTerm::GreaterEquals: op = QIfFilterTerm::LowerEquals; break;
                        case QIfFilterTerm::GreaterThan: op = QIfFilterTerm::LowerThan; break;
                        case QIfFilterTerm::LowerEquals: op = QIfFilterTerm::GreaterEquals; break;
                        case QIfFilterTerm::LowerThan: op = QIfFilterTerm::GreaterThan; break;
                        default: qFatal("The Grammer was changed but not all logic was ported properly");
                    }

                    QIfFilterTerm *term = new QIfFilterTerm();
                    term->d_func()->m_property = sym(3).toString();
                    term->d_func()->m_operator = op;
                    term->d_func()->m_value = sym(1);
                    m_termStack.push(term);
              } break;

              case 25: {
                  m_operatorStack.push(QIfFilterTerm::GreaterEquals);
              } break;

              case 26: {
                  m_operatorStack.push(QIfFilterTerm::GreaterThan);
              } break;

              case 27: {
                  m_operatorStack.push(QIfFilterTerm::LowerEquals);
              } break;

              case 28: {
                  m_operatorStack.push(QIfFilterTerm::LowerThan);
              } break;

              case 31: {
                  m_operatorStack.push(QIfFilterTerm::EqualsCaseInsensitive);
              } break;

              case 32: {
                  m_operatorStack.push(QIfFilterTerm::Equals);
              } break;

              case 33: {
                  m_operatorStack.push(QIfFilterTerm::Equals);
              } break;

              case 34: {
                  m_operatorStack.push(QIfFilterTerm::Unequals);
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
            Q_UNUSED(reduces);

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

void QIfQueryParser::setErrorString(const QString &error)
{
    int err_col = column - yyleng;

    m_error = error;

    m_error.append(m_query).append(QLatin1String("\n"));
    QString marker(QLatin1String("^"));

    for (int i=0; i<err_col; i++)
        marker.prepend(QLatin1String(" "));

    for (int i=0; i<yyleng - 1; i++)
        marker.append(QLatin1String("-"));

    m_error.append(marker);
}

QT_END_NAMESPACE

