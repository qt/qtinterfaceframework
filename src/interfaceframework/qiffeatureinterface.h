// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFFEATUREINTERFACE_H
#define QIFFEATUREINTERFACE_H

#include <QtInterfaceFramework/QIfAbstractFeature>
#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfFeatureInterface : public QObject
{
    Q_OBJECT

public:
    explicit QIfFeatureInterface(QObject *parent = nullptr);

    virtual void initialize() = 0;

protected:
    QIfFeatureInterface(QObjectPrivate &dd, QObject *parent = nullptr);

Q_SIGNALS:
    void errorChanged(QIfAbstractFeature::Error error, const QString &message = QString());
    void initializationDone();
};

QT_END_NAMESPACE

#endif // QIFFEATUREINTERFACE_H
