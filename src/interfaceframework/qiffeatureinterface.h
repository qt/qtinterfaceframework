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
