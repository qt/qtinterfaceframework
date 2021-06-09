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

#ifndef QIfStandardItem_H
#define QIfStandardItem_H

#include <QtCore/QMetaType>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QVariantMap>
#include <QtCore/qobjectdefs.h>
#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class QIfStandardItemPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfStandardItem
{
    Q_GADGET

    Q_PROPERTY(QString id READ id WRITE setId)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QVariantMap data READ data WRITE setData)

public:
    QIfStandardItem();
    QIfStandardItem(const QIfStandardItem &);
    QIfStandardItem &operator=(const QIfStandardItem &);
    virtual ~QIfStandardItem();

    virtual QString id() const;
    virtual void setId(const QString &id);
    virtual QString name() const;
    virtual QString type() const;
    virtual QVariantMap data() const;
    virtual void setData(const QVariantMap &data);

    bool operator==(const QIfStandardItem &other) const;
    inline bool operator!=(const QIfStandardItem &other) const { return !(*this == other); }

private:
    QSharedDataPointer<QIfStandardItemPrivate> d;
};

Q_DECLARE_TYPEINFO(QIfStandardItem, Q_MOVABLE_TYPE);

using QIfFilterAndBrowseModelItem = QIfStandardItem;

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIfStandardItem)

#endif // QIfStandardItem_H
