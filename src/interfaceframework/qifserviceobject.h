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
