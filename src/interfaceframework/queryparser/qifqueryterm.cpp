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

#include "qifqueryterm.h"
#include "qifqueryterm_p.h"

#include <QDataStream>
#include <QMetaEnum>
#include <QtDebug>

QT_BEGIN_NAMESPACE

QIfConjunctionTermPrivate::QIfConjunctionTermPrivate()
    : m_conjunction(QIfConjunctionTerm::And)
{
}

QIfScopeTermPrivate::QIfScopeTermPrivate()
    : m_term(nullptr)
    , m_negated(false)
{
}

QIfFilterTermPrivate::QIfFilterTermPrivate()
    : m_operator(QIfFilterTerm::Equals)
    , m_negated(false)
{
}

QString QIfFilterTermPrivate::operatorToString() const
{
    switch (m_operator){
    case QIfFilterTerm::Equals: return QLatin1String("=");
    case QIfFilterTerm::EqualsCaseInsensitive: return QLatin1String("~=");
    case QIfFilterTerm::Unequals: return QLatin1String("!=");
    case QIfFilterTerm::GreaterThan: return QLatin1String(">");
    case QIfFilterTerm::GreaterEquals: return QLatin1String(">=");
    case QIfFilterTerm::LowerThan: return QLatin1String("<");
    case QIfFilterTerm::LowerEquals: return QLatin1String("<=");
    }

    return QLatin1String("unknown type");
}

QIfOrderTermPrivate::QIfOrderTermPrivate()
    : m_ascending(false)
{
}

QIfOrderTermPrivate::QIfOrderTermPrivate(const QIfOrderTermPrivate &other)
    : QSharedData(other)
    , m_ascending(other.m_ascending)
    , m_propertyName(other.m_propertyName)
{

}

/*!
    \class QIfAbstractQueryTerm
    \inmodule QtInterfaceFramework
    \brief The base class of all query terms.

    Following terms are supported:
    \annotatedlist qtif_queryterms

    See \l {Qt Interface Framework Query Language} for how it can be used.
*/

/*!
    \enum QIfAbstractQueryTerm::Type
    \value FilterTerm
           A filter term stands for a filter which checks a specific identifier against a given value.
    \value ConjunctionTerm
           A conjunction term can combine multiple terms together, either by a OR or an AND conjunction.
    \value ScopeTerm
           A scope term is used to group terms together, e.g. to apply a negation to a group.
*/

/*!
    \fn QIfAbstractQueryTerm::Type QIfAbstractQueryTerm::type() const

    Returns the type of this query term.
*/

/*!
    \fn QString QIfAbstractQueryTerm::toString() const

    Returns a string representation of the query.
*/

QIfAbstractQueryTerm::~QIfAbstractQueryTerm()
{
}

/*!
    \class QIfConjunctionTerm
    \inmodule QtInterfaceFramework
    \ingroup qtif_queryterms
    \brief The QIfConjunctionTerm is the representation of a conjunction between two query terms.
*/

/*!
    \enum QIfConjunctionTerm::Conjunction
    \value And
           The AND conjunction combines the filters to only match when all supplied filters are \c true.
    \value Or
           The OR conjunction combines the filters to match when one of the supplied filters are \c true.
*/

QIfConjunctionTerm::QIfConjunctionTerm()
    : d_ptr(new QIfConjunctionTermPrivate)
{
}

QIfConjunctionTerm::~QIfConjunctionTerm()
{
    Q_D(QIfConjunctionTerm);
    qDeleteAll(d->m_terms);
    delete d_ptr;
}

/*!
    \reimp
*/
QIfAbstractQueryTerm::Type QIfConjunctionTerm::type() const
{
    return QIfAbstractQueryTerm::ConjunctionTerm;
}

/*!
    \reimp
*/
QString QIfConjunctionTerm::toString() const
{
    Q_D(const QIfConjunctionTerm);
    const QChar conjunction = d->m_conjunction == Or ? QLatin1Char('|') : QLatin1Char('&');

    QString string;
    if (!d->m_terms.empty()) {
        for (QIfAbstractQueryTerm *term : d->m_terms)
            string += term->toString() + QLatin1Char(' ') + conjunction + QLatin1Char(' ');
        string.chop(3); // remove trailing " & " or " | "
    }
    return string;
}

/*!
    Returns the type of the conjunction.
*/
QIfConjunctionTerm::Conjunction QIfConjunctionTerm::conjunction() const
{
    Q_D(const QIfConjunctionTerm);
    return d->m_conjunction;
}

/*!
    Returns the terms which are conjuncted together.
*/
QList<QIfAbstractQueryTerm *> QIfConjunctionTerm::terms() const
{
    Q_D(const QIfConjunctionTerm);
    return d->m_terms;
}

/*!
    \class QIfScopeTerm
    \inmodule QtInterfaceFramework
    \ingroup qtif_queryterms
    \brief The QIfScopeTerm is the representation of a scope which can hold another term.
*/
QIfScopeTerm::QIfScopeTerm()
    : d_ptr(new QIfScopeTermPrivate)
{
}

QIfScopeTerm::~QIfScopeTerm()
{
    Q_D(QIfScopeTerm);
    delete d->m_term;
    delete d_ptr;
}

/*!
    \reimp
*/
QIfAbstractQueryTerm::Type QIfScopeTerm::type() const
{
    return QIfAbstractQueryTerm::ScopeTerm;
}

/*!
    \reimp
*/
QString QIfScopeTerm::toString() const
{
    Q_D(const QIfScopeTerm);
    QString string = QLatin1String("(") + d->m_term->toString() + QLatin1String(")");
    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

/*!
    Returns \c true when this term is negated, otherwise \c false
*/
bool QIfScopeTerm::isNegated() const
{
    Q_D(const QIfScopeTerm);
    return d->m_negated;
}

/*!
    Returns the term which is inside this scope.

    This term can be a conjunction term if there are multiple terms inside.
*/
QIfAbstractQueryTerm *QIfScopeTerm::term() const
{
    Q_D(const QIfScopeTerm);
    return d->m_term;
}

/*!
    \class QIfFilterTerm
    \inmodule QtInterfaceFramework
    \ingroup qtif_queryterms
    \brief The QIfFilterTerm is the representation of a filter.

    The filter is either in the form:

    \code
    identifier operator value
    \endcode

    or:

    \code
    value operator identifier
    \endcode
*/

/*!
    \enum QIfFilterTerm::Operator
    \value Equals
           Tests whether the value from the identifier is the equal to the passed value. In case of a string the comparison is case-senstitive.
    \value EqualsCaseInsensitive
           Tests whether the value from the identifier is the equal to the passed value, but the comparison is done case-insensitive.
    \value Unequals
           Tests whether the value from the identifier is the unequal to the passed value. In case of a string the comparison is case-senstitive.
    \value GreaterThan
           Tests whether the value from the identifier is greater than the passed value. This does only work for numbers.
    \value GreaterEquals
           Tests whether the value from the identifier is greater than or equal to the passed value. This does only work for numbers.
    \value LowerThan
           Tests whether the value from the identifier is lower than the passed value. This does only work for numbers.
    \value LowerEquals
           Tests whether the value from the identifier is lower than or equal to the passed value. This does only work for numbers.
*/

QIfFilterTerm::QIfFilterTerm()
    : d_ptr(new QIfFilterTermPrivate)
{
}

QIfFilterTerm::~QIfFilterTerm()
{
    delete d_ptr;
}

/*!
    \reimp
*/
QIfAbstractQueryTerm::Type QIfFilterTerm::type() const
{
    return QIfAbstractQueryTerm::FilterTerm;
}

/*!
    \reimp
*/
QString QIfFilterTerm::toString() const
{
    Q_D(const QIfFilterTerm);
    QString string;

    string = d->m_property + d->operatorToString() + d->m_value.toString();

    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

/*!
    Returns the operator of this filter.
*/
QIfFilterTerm::Operator QIfFilterTerm::operatorType() const
{
    Q_D(const QIfFilterTerm);
    return d->m_operator;
}

/*!
    Returns the value of the filter.
*/
QVariant QIfFilterTerm::value() const
{
    Q_D(const QIfFilterTerm);
    return d->m_value;
}

/*!
    Returns the property this filter should act on.
*/
QString QIfFilterTerm::propertyName() const
{
    Q_D(const QIfFilterTerm);
    return d->m_property;
}

/*!
    Returns \c true when this term is negated, otherwise \c false
*/
bool QIfFilterTerm::isNegated() const
{
    Q_D(const QIfFilterTerm);
    return d->m_negated;
}

/*!
    \class QIfOrderTerm
    \inmodule QtInterfaceFramework
    \brief The QIfOrderTerm is the representation of a scope which can hold another term.
*/
QIfOrderTerm::QIfOrderTerm()
    : d(new QIfOrderTermPrivate)
{
}

QIfOrderTerm::QIfOrderTerm(const QIfOrderTerm &other)
    : d(other.d)
{
}

QIfOrderTerm::~QIfOrderTerm()
{
}

QIfOrderTerm &QIfOrderTerm::operator =(const QIfOrderTerm &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns \c true when it should be sorted in ascending order.
    Returns \c false when it should be sorted in descending order.
*/
bool QIfOrderTerm::isAscending() const
{
    return d->m_ascending;
}

/*!
    Returns the property which should be used for sorting.
*/
QString QIfOrderTerm::propertyName() const
{
    return d->m_propertyName;
}

QDataStream &operator<<(QDataStream &out, QIfConjunctionTerm::Conjunction var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIfConjunctionTerm::Conjunction &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIfConjunctionTerm::Conjunction>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIfConjunctionTerm::Conjunction, value =" << val;
    var = QIfConjunctionTerm::Conjunction(val);
    return in;
}

QDataStream &operator<<(QDataStream &out, QIfFilterTerm::Operator var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIfFilterTerm::Operator &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIfFilterTerm::Operator>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIfFilterTerm::Operator, value =" << val;
    var = QIfFilterTerm::Operator(val);
    return in;
}

QDataStream &operator<<(QDataStream &out, QIfAbstractQueryTerm *var)
{
    if (var->type() == QIfAbstractQueryTerm::FilterTerm) {
        auto *term = static_cast<QIfFilterTerm*>(var);
        out << QStringLiteral("filter");
        out << term->operatorType();
        out << term->value();
        out << term->propertyName();
        out << term->isNegated();
    } else if (var->type() == QIfAbstractQueryTerm::ScopeTerm) {
        auto *term = static_cast<QIfScopeTerm*>(var);
        out << QStringLiteral("scope");
        out << term->isNegated();
        out << term->term();
    } else {
        auto *term = static_cast<QIfConjunctionTerm*>(var);
        out << QStringLiteral("conjunction");
        out << term->conjunction();
        const auto subTerms = term->terms();
        out << subTerms.count();
        for (const auto subTerm : subTerms)
            out << subTerm;
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, QIfAbstractQueryTerm **var)
{
    QString type;
    QIfAbstractQueryTerm *aTerm=nullptr;
    in >> type;
    if (type == QStringLiteral("filter")) {
        auto term = new QIfFilterTerm();
        aTerm = term;
        in >> term->d_ptr->m_operator;
        in >> term->d_ptr->m_value;
        in >> term->d_ptr->m_property;
        in >> term->d_ptr->m_negated;
    } else if (type == QStringLiteral("scope")) {
        auto term = new QIfScopeTerm();
        aTerm = term;
        in >> term->d_ptr->m_negated;
        in >> &term->d_ptr->m_term;
    } else {
        Q_ASSERT(type == QStringLiteral("conjunction"));
        auto term = new QIfConjunctionTerm();
        aTerm = term;
        qsizetype count = 0;
        in >> term->d_ptr->m_conjunction;
        in >> count;
        for (int i = 0; i < count; ++i) {
            QIfAbstractQueryTerm *subTerm=nullptr;
            in >> &subTerm;
            term->d_ptr->m_terms.append(subTerm);
        }
    }
    *var = aTerm;
    return in;
}

QDataStream &operator<<(QDataStream &out, const QIfOrderTerm &var)
{
    out << var.propertyName();
    out << var.isAscending();
    return out;
}

QDataStream &operator>>(QDataStream &in, QIfOrderTerm &var)
{
    in >> var.d->m_propertyName;
    in >> var.d->m_ascending;
    return in;
}

QT_END_NAMESPACE
