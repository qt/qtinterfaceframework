/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#ifndef QIVIPENDINGREPLY_H
#define QIVIPENDINGREPLY_H

#include <QObject>
#include <QVariant>
#include <QJSValue>
#include <QSharedPointer>

#include <QtIviCore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class QIviPendingReplyWatcherPrivate;

class Q_QTIVICORE_EXPORT QIviPendingReplyWatcher : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(bool valid READ isValid CONSTANT)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable NOTIFY valueChanged)
    Q_PROPERTY(bool success READ isSuccessful NOTIFY valueChanged)

public:
    ~QIviPendingReplyWatcher();

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
    explicit QIviPendingReplyWatcher(int userType);
    Q_DECLARE_PRIVATE(QIviPendingReplyWatcher)
    friend class QIviPendingReplyBase;
};

class Q_QTIVICORE_EXPORT QIviPendingReplyBase
{
    Q_GADGET
    Q_PROPERTY(QIviPendingReplyWatcher* watcher READ watcher)
    Q_PROPERTY(QVariant value READ value)
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(bool resultAvailable READ isResultAvailable)
    Q_PROPERTY(bool success READ isSuccessful)

public:
    QIviPendingReplyBase();
    explicit QIviPendingReplyBase(int userType);
    QIviPendingReplyBase(const QIviPendingReplyBase & other);
    virtual ~QIviPendingReplyBase();

    QIviPendingReplyWatcher* watcher() const;
    QVariant value() const;
    bool isValid() const;
    bool isResultAvailable() const;
    bool isSuccessful() const;

    Q_INVOKABLE void then(const QJSValue &success, const QJSValue &failed = QJSValue());
    Q_INVOKABLE void setSuccess(const QVariant & value);
    Q_INVOKABLE void setFailed();

protected:
    void setSuccessNoCheck(const QVariant & value);

    QSharedPointer<QIviPendingReplyWatcher> m_watcher;
};

template <typename T> class QIviPendingReply : public QIviPendingReplyBase
{
public:
    QIviPendingReply()
        : QIviPendingReplyBase(qMetaTypeId<T>())
    {}

    void setSuccess(const T &value)
    {
        setSuccessNoCheck(QVariant::fromValue(value));
    }

    T reply() const { return m_watcher->value().template value<T>(); }

    static QIviPendingReply createFailedReply()
    {
        QIviPendingReply<T> reply;
        reply.setFailed();
        return reply;
    }
};

template <> class QIviPendingReply <QVariant> : public QIviPendingReplyBase
{
public:
    QIviPendingReply()
        : QIviPendingReplyBase(qMetaTypeId<QVariant>())
    {}

    void setSuccess(const QVariant &value)
    {
        setSuccessNoCheck(value);
    }

    QVariant reply() const { return m_watcher->value(); }

    static QIviPendingReply createFailedReply()
    {
        QIviPendingReply<QVariant> reply;
        reply.setFailed();
        return reply;
    }
};

template <> class QIviPendingReply <void> : public QIviPendingReplyBase
{
public:
    QIviPendingReply()
        : QIviPendingReplyBase(qMetaTypeId<void>())
    {}

    void setSuccess()
    {
        setSuccessNoCheck(QVariant());
    }

    void reply() const { return; }

    static QIviPendingReply createFailedReply()
    {
        QIviPendingReply<void> reply;
        reply.setFailed();
        return reply;
    }
};

template <typename T> void qIviRegisterPendingReplyType(const char *name = nullptr)
{
    qRegisterMetaType<T>();
    const char* n = name ? name : QMetaType::typeName(qMetaTypeId<T>());
    const QString t_name = QStringLiteral("QIviPendingReply<") + QLatin1String(n) + QStringLiteral(">");
    qRegisterMetaType<QIviPendingReplyBase>(qPrintable(t_name));
}

QT_END_NAMESPACE

#endif // QIVIPENDINGREPLY_H
