// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSERVICEOBJECT_H
#define QIFSERVICEOBJECT_H

#include <QtCore/QObject>
#include <QtInterfaceFramework/QIfServiceInterface>
#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceObject : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIfServiceInterface)

    Q_PROPERTY(QString id READ id CONSTANT)

public:
    explicit QIfServiceObject(QObject *parent = nullptr);

    virtual QString id() const;

private:
    QString m_id;
};

QT_END_NAMESPACE

#endif // QIFSERVICEOBJECT_H
