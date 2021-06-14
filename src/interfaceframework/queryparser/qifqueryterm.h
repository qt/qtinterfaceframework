/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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
****************************************************************************/

#ifndef QUERYTERM_H
#define QUERYTERM_H

#include <QObject>
#include <QSharedData>
#include <QVariant>

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
