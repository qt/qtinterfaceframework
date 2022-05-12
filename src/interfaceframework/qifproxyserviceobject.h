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

#ifndef QIFPROXYSERVICEOBJECT_H
#define QIFPROXYSERVICEOBJECT_H

#include <QtInterfaceFramework/QIfServiceObject>
#include <QtInterfaceFramework/qtifglobal.h>

class ServiceManagerTest;

QT_BEGIN_NAMESPACE

class QIfProxyServiceObjectPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfProxyServiceObject : public QIfServiceObject
{
    Q_OBJECT

public:
    explicit QIfProxyServiceObject(QIfServiceInterface *interface);
    explicit QIfProxyServiceObject(const QHash<QString, QIfFeatureInterface*> &interfaceMap);

    QStringList interfaces() const override;
    QIfFeatureInterface *interfaceInstance(const QString &interface) const override;

private:
    Q_DECLARE_PRIVATE(QIfProxyServiceObject)
    QIfProxyServiceObjectPrivate *d_ptr;
    friend class ::ServiceManagerTest;
};

QT_END_NAMESPACE

#endif // QIFPROXYSERVICEOBJECT_H
