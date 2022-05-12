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

#ifndef QIFPAGINGMODELINTERFACE_H
#define QIFPAGINGMODELINTERFACE_H

#include <QUuid>
#include <QtInterfaceFramework/QIfFeatureInterface>
#include <QtInterfaceFramework/QIfPagingModel>
#include <QtInterfaceFramework/QtInterfaceFrameworkModule>

QT_BEGIN_NAMESPACE

class QIfPagingModelInterfacePrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPagingModelInterface : public QIfFeatureInterface
{
    Q_OBJECT

public:
    explicit QIfPagingModelInterface(QObject *parent = nullptr);

    virtual void registerInstance(const QUuid &identifier) = 0;
    virtual void unregisterInstance(const QUuid &identifier) = 0;

    virtual void fetchData(const QUuid &identifier, int start, int count) = 0;

protected:
    QIfPagingModelInterface(QObjectPrivate &dd, QObject *parent = nullptr);

Q_SIGNALS:
    void supportedCapabilitiesChanged(const QUuid &identifier, QtInterfaceFrameworkModule::ModelCapabilities capabilities);
    void countChanged(const QUuid &identifier = QUuid(), int count = -1);
    void dataFetched(const QUuid &identifier, const QList<QVariant> &data, int start, bool moreAvailable);
    void dataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count);
};

#define QIfPagingModel_iid "org.qt-project.interfaceframework.PagingModel/1.0"

QT_END_NAMESPACE

#endif // QIFPAGINGMODELINTERFACE_H
