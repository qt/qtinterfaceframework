// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
