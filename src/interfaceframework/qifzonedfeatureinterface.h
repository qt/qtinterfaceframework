// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#ifndef QIFZONEDFEATUREINTERFACE_H
#define QIFZONEDFEATUREINTERFACE_H

#include <QtInterfaceFramework/QIfAbstractZonedFeature>
#include <QtInterfaceFramework/QIfFeatureInterface>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfZonedFeatureInterface : public QIfFeatureInterface
{
    Q_OBJECT

public:
    explicit QIfZonedFeatureInterface(QObject *parent = nullptr);

    virtual QStringList availableZones() const = 0;

Q_SIGNALS:
    void availableZonesChanged(const QStringList &zones);
};

QT_END_NAMESPACE

#endif // QIFZONEDFEATUREINTERFACE_H

