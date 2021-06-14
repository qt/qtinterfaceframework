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

#ifndef QIFPENDINGREPLY_H
#define QIFPENDINGREPLY_H

#include <QJSValue>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>
#include <QDebug>
#include <QMetaEnum>

#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class QIfPendingReplyWatcherPrivate;

Q_QTINTERFACEFRAMEWORK_EXPORT void qifRegisterPendingReplyBasicTypes();

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPendingReplyWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(bool valid READ isValid CONSTANT)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable NOTIFY valueChanged)
    Q_PROPERTY(bool success READ isSuccessful NOTIFY valueChanged)

public:
    QVariant value() const;
    bool isValid() const;
    bool isResultAvailable() const;
    bool isSuccessful() const;

    Q_INVOKABLE void setSuccess(const QVariant &value);
    Q_INVOKABLE void setFailed();
    Q_INVOKABLE void then(const QJSValue &success, const QJSValue &failed = QJSValue());

Q_SIGNALS:
    void replyFailed();
    void replySuccess();
    void valueChanged(const QVariant &value);

private:
    explicit QIfPendingReplyWatcher(int userType);
    Q_DECLARE_PRIVATE(QIfPendingReplyWatcher)
    friend class QIfPendingReplyBase;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPendingReplyBase
{
    Q_GADGET
    Q_PROPERTY(QIfPendingReplyWatcher* watcher READ watcher)
    Q_PROPERTY(QVariant value READ value)
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable)
    Q_PROPERTY(bool success READ isSuccessful)

public:
    explicit QIfPendingReplyBase(int userType);
    QIfPendingReplyBase() = default;
    QIfPendingReplyBase(const QIfPendingReplyBase & other);
    ~QIfPendingReplyBase() = default;
    QIfPendingReplyBase& operator=(const QIfPendingReplyBase&) = default;
    QIfPendingReplyBase& operator=(QIfPendingReplyBase&&) = default;

    QIfPendingReplyWatcher* watcher() const;
    QVariant value() const;
    bool isValid() const;
    bool isResultAvailable() const;
    bool isSuccessful() const;

    Q_INVOKABLE void then(const QJSValue &success, const QJSValue &failed = QJSValue());
    Q_INVOKABLE void setSuccess(const QVariant & value);
    Q_INVOKABLE void setFailed();

protected:
    void setSuccessNoCheck(const QVariant & value);

    QSharedPointer<QIfPendingReplyWatcher> m_watcher;
};

template <typename T> class QIfPendingReply : public QIfPendingReplyBase
{
public:
    QIfPendingReply(const T &successValue)
        : QIfPendingReply()
    {
        setSuccess(successValue);
    }

    QIfPendingReply()
        : QIfPendingReplyBase(qMetaTypeId<T>())
    {}

    using QIfPendingReplyBase::setSuccess;

    void setSuccess(const T &val)
    {
        setSuccessNoCheck(QVariant::fromValue(val));
    }

    T reply() const { return m_watcher->value().template value<T>(); }

    using QIfPendingReplyBase::then;

    void then(const std::function<void(const T &)> &success, const std::function<void()> &failed = std::function<void()>()) {
        if (isResultAvailable()) {
            if (isSuccessful() && success)
                success(reply());
            else if (failed)
                failed();
        } else {
            QSharedPointer<QIfPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    success(w->value().value<T>());
                });
            }
            if (failed) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replyFailed, watcher(), [failed]() {
                    failed();
                });
            }
        }
    }

    static QIfPendingReply createFailedReply()
    {
        QIfPendingReply<T> reply;
        reply.setFailed();
        return reply;
    }
};

template <> class QIfPendingReply <QVariant> : public QIfPendingReplyBase
{
public:
    QIfPendingReply(const QVariant &successValue)
        : QIfPendingReply()
    {
        setSuccess(successValue);
    }

    QIfPendingReply()
        : QIfPendingReplyBase(qMetaTypeId<QVariant>())
    {}

    void setSuccess(const QVariant &val)
    {
        setSuccessNoCheck(val);
    }

    QVariant reply() const { return m_watcher->value(); }

    void then(const std::function<void(const QVariant &)> &success, const std::function<void()> &failed = std::function<void()>()) {
        if (isResultAvailable()) {
            if (isSuccessful() && success)
                success(reply());
            else if (failed)
                failed();
        } else {
            QSharedPointer<QIfPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    success(w->value());
                });
            }
            if (failed) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replyFailed, watcher(), [failed]() {
                    failed();
                });
            }
        }
    }

    static QIfPendingReply createFailedReply()
    {
        QIfPendingReply<QVariant> reply;
        reply.setFailed();
        return reply;
    }
};

template <> class QIfPendingReply <void> : public QIfPendingReplyBase
{
public:
    QIfPendingReply()
        : QIfPendingReplyBase(qMetaTypeId<void>())
    {}

    using QIfPendingReplyBase::setSuccess;

    void setSuccess()
    {
        setSuccessNoCheck(QVariant());
    }

    void reply() const { return; }

    void then(const std::function<void()> &success, const std::function<void()> &failed = std::function<void()>()) {
        if (isResultAvailable()) {
            if (isSuccessful() && success)
                success();
            else if (failed)
                failed();
        } else {
            QSharedPointer<QIfPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    success();
                });
            }
            if (failed) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replyFailed, watcher(), [failed]() {
                    failed();
                });
            }
        }
    }

    static QIfPendingReply createFailedReply()
    {
        QIfPendingReply<void> reply;
        reply.setFailed();
        return reply;
    }
};

//Workaround for QTBUG-83664
//If T is a enum
template <typename T> Q_INLINE_TEMPLATE typename std::enable_if<QtPrivate::IsQEnumHelper<T>::Value, void>::type qIfRegisterPendingReplyType(const char *name = nullptr)
{
    qRegisterMetaType<T>();
    QString n;
    if (name) {
        n = QLatin1String(name);
    } else {
        QMetaEnum me = QMetaEnum::fromType<T>();
        if (me.isValid() && me.isFlag())
            n = QLatin1String(me.scope()) + QStringLiteral("::") + QLatin1String(me.name());
        else
            n = QLatin1String(QMetaType(qMetaTypeId<T>()).name());
    }

    const QString t_name = QStringLiteral("QIfPendingReply<") + n + QStringLiteral(">");
    qRegisterMetaType<QIfPendingReplyBase>(qPrintable(t_name));
}

//If T is NOT a enum
template <typename T> Q_INLINE_TEMPLATE typename std::enable_if<!QtPrivate::IsQEnumHelper<T>::Value, void>::type qIfRegisterPendingReplyType(const char *name = nullptr)
{
    qRegisterMetaType<T>();
    const char* n = name ? name : QMetaType(qMetaTypeId<T>()).name();
    const QString t_name = QStringLiteral("QIfPendingReply<") + QLatin1String(n) + QStringLiteral(">");
    qRegisterMetaType<QIfPendingReplyBase>(qPrintable(t_name));
}

QT_END_NAMESPACE

#endif // QIFPENDINGREPLY_H
