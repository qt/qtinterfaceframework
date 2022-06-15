// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
