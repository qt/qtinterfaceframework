// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSIMULATIONGLOBALOBJECT_P_H
#define QIFSIMULATIONGLOBALOBJECT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qifqmlconversion_helper.h>
#include <QIfPagingModelInterface>

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QMetaType>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaEnum>
#include <QJSValue>
#include <QQmlPropertyMap>

#include <QDebug>
#include <private/qglobal_p.h>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfSimulationGlobalObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant simulationData READ simulationData CONSTANT FINAL)

public:
    explicit QIfSimulationGlobalObject(QObject *parent = nullptr);

    QVariant simulationData() const;
    void setSimulationData(const QVariant &simulationData);

    Q_INVOKABLE QVariantMap findData(const QVariantMap &data, const QString &interface);
    Q_INVOKABLE void initializeDefault(const QVariantMap &data, QObject *object);
    Q_INVOKABLE QVariant defaultValue(const QVariantMap &data, const QString &zone = QString());
    Q_INVOKABLE QString constraint(const QVariantMap &data, const QString &zone = QString());
    Q_INVOKABLE bool checkSettings(const QVariantMap &data, const QVariant &value, const QString &zone = QString());
    Q_INVOKABLE QVariant parseDomainValue(const QVariantMap &data, const QString &domain, const QString &zone = QString());

private:
    QGenericArgument createArgument(const QVariant &variant);
    QVariant m_simulationData;
};

QT_END_NAMESPACE

#endif // QIFSIMULATIONGLOBALOBJECT_P_H
