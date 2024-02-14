// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPENDINGREPLY_H
#define QIFPENDINGREPLY_H

#include <QtQml/QJSValue>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>
#include <QtCore/QDebug>
#include <QtCore/QMetaEnum>

#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class QIfPendingReplyWatcherPrivate;

Q_QTINTERFACEFRAMEWORK_EXPORT void qifRegisterPendingReplyBasicTypes();

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPendingReplyWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged FINAL)
    Q_PROPERTY(bool valid READ isValid CONSTANT FINAL)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable NOTIFY valueChanged FINAL)
    Q_PROPERTY(bool success READ isSuccessful NOTIFY valueChanged FINAL)

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
    Q_PROPERTY(QIfPendingReplyWatcher* watcher READ watcher FINAL)
    Q_PROPERTY(QVariant value READ value FINAL)
    Q_PROPERTY(bool valid READ isValid FINAL)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable FINAL)
    Q_PROPERTY(bool success READ isSuccessful FINAL)

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
            QWeakPointer<QIfPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    if (w)
                        success(w.toStrongRef()->value().value<T>());
                });
            }
            if (failed)
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replyFailed, watcher(), failed);
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
            QWeakPointer<QIfPendingReplyWatcher> w = m_watcher;
            if (success) {
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replySuccess, watcher(), [success, w]() {
                    if (w)
                        success(w.toStrongRef()->value());
                });
            }
            if (failed)
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replyFailed, watcher(), failed);
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
            if (success)
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replySuccess, watcher(), success);
            if (failed)
                QObject::connect(watcher(), &QIfPendingReplyWatcher::replyFailed, watcher(), failed);
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
            n = QLatin1String(me.scope()) + QLatin1String("::") + QLatin1String(me.name());
        else
            n = QLatin1String(QMetaType(qMetaTypeId<T>()).name());
    }

    const QString t_name = QLatin1String("QIfPendingReply<") + n + QLatin1String(">");
    qRegisterMetaType<QIfPendingReplyBase>(qPrintable(t_name));
}

//If T is NOT a enum
template <typename T> Q_INLINE_TEMPLATE typename std::enable_if<!QtPrivate::IsQEnumHelper<T>::Value, void>::type qIfRegisterPendingReplyType(const char *name = nullptr)
{
    qRegisterMetaType<T>();
    const char* n = name ? name : QMetaType(qMetaTypeId<T>()).name();
    const QString t_name = QLatin1String("QIfPendingReply<") + QLatin1String(n) + QLatin1String(">");
    qRegisterMetaType<QIfPendingReplyBase>(qPrintable(t_name));
}

QT_END_NAMESPACE

#endif // QIFPENDINGREPLY_H
