// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QUERYTERM_H
#define QUERYTERM_H

#include <QtCore/QObject>
#include <QtCore/QSharedData>
#include <QtCore/QVariant>

#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractQueryTerm
{
public:
    enum Type {
        FilterTerm,
        ConjunctionTerm,
        ScopeTerm
    };

    virtual ~QIfAbstractQueryTerm();

    virtual QString toString() const = 0;
    virtual QIfAbstractQueryTerm::Type type() const = 0;
};

class QIfConjunctionTermPrivate;
class Q_QTINTERFACEFRAMEWORK_EXPORT QIfConjunctionTerm : public QIfAbstractQueryTerm
{
    Q_GADGET
public:
    enum Conjunction {
        And,
        Or
    };
    Q_ENUM(Conjunction)

    explicit QIfConjunctionTerm();
    ~QIfConjunctionTerm() override;

    QIfAbstractQueryTerm::Type type() const override;
    QString toString() const override;
    Conjunction conjunction() const;
    QList<QIfAbstractQueryTerm*> terms() const;

private:
    Q_DISABLE_COPY(QIfConjunctionTerm)
    QIfConjunctionTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIfConjunctionTerm)
    friend class QIfQueryParser;
    friend Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfAbstractQueryTerm** var);
};

class QIfScopeTermPrivate;
class Q_QTINTERFACEFRAMEWORK_EXPORT QIfScopeTerm : public QIfAbstractQueryTerm
{
    Q_GADGET
public:

    explicit QIfScopeTerm();
    ~QIfScopeTerm() override;

    QIfAbstractQueryTerm::Type type() const override;
    QString toString() const override;
    bool isNegated() const;
    QIfAbstractQueryTerm* term() const;

private:
    Q_DISABLE_COPY(QIfScopeTerm)
    QIfScopeTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIfScopeTerm)
    friend class QIfQueryParser;
    friend Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfAbstractQueryTerm** var);
};

class QIfFilterTermPrivate;
class Q_QTINTERFACEFRAMEWORK_EXPORT QIfFilterTerm : public QIfAbstractQueryTerm
{
    Q_GADGET
public:
    enum Operator {
        Equals,
        EqualsCaseInsensitive,
        Unequals,
        GreaterThan,
        GreaterEquals,
        LowerThan,
        LowerEquals
    };
    Q_ENUM(Operator)

    explicit QIfFilterTerm();
    ~QIfFilterTerm() override;

    QIfAbstractQueryTerm::Type type() const override;
    QString toString() const override;
    Operator operatorType() const;
    QVariant value() const;
    QString propertyName() const;
    bool isNegated() const;

private:
    Q_DISABLE_COPY(QIfFilterTerm)
    QIfFilterTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIfFilterTerm)
    friend class QIfQueryParser;
    friend Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfAbstractQueryTerm** var);
};

class QIfOrderTermPrivate;
class Q_QTINTERFACEFRAMEWORK_EXPORT QIfOrderTerm
{
public:
    QIfOrderTerm();
    QIfOrderTerm(const QIfOrderTerm &other);
    virtual ~QIfOrderTerm();
    QIfOrderTerm& operator =(const QIfOrderTerm &other);

    bool isAscending() const;
    QString propertyName() const;

private:
    QSharedDataPointer<QIfOrderTermPrivate> d;
    friend class QIfQueryParser;
    friend Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfOrderTerm &var);
};

Q_DECLARE_TYPEINFO(QIfOrderTerm, Q_MOVABLE_TYPE);

Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator<<(QDataStream &out, QIfConjunctionTerm::Conjunction var);
Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfConjunctionTerm::Conjunction &var);
Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator<<(QDataStream &out, QIfFilterTerm::Operator var);
Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfFilterTerm::Operator &var);
Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator<<(QDataStream &out, QIfAbstractQueryTerm *var);
Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfAbstractQueryTerm **var);
Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator<<(QDataStream &out, const QIfOrderTerm &var);
Q_QTINTERFACEFRAMEWORK_EXPORT QDataStream &operator>>(QDataStream &in, QIfOrderTerm &var);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIfOrderTerm)

#endif // QUERYTERM_H
