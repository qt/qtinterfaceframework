// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSIMULATIONENGINE_H
#define QIFSIMULATIONENGINE_H

#include <QtInterfaceFramework/QtInterfaceFrameworkModule>

#include <QtQml/QQmlApplicationEngine>
#include <QtInterfaceFramework/qifsimulationproxy.h>
#include <QtInterfaceFramework/qifqmlconversion_helper.h>

QT_BEGIN_NAMESPACE

class QIfSimulationGlobalObject;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfSimulationEngine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit QIfSimulationEngine(QObject *parent = nullptr);
    explicit QIfSimulationEngine(const QString &identifier, QObject *parent = nullptr);

    template <typename T> void registerSimulationInstance(T* instance, const char *uri, int versionMajor, int versionMinor, const char *qmlName)
    {
        qtif_private::QIfSimulationProxy<T>::buildMetaObject();
        //pass engine here to check that it's only used in this engine
        qtif_private::QIfSimulationProxy<T>::registerInstance(this, instance);
        qmlRegisterType< qtif_private::QIfSimulationProxy<T> >(uri, versionMajor, versionMinor, qmlName);
    }

    void loadSimulationData(const QString &dataFile);
    void loadSimulation(const QUrl &file);

private:
    QIfSimulationGlobalObject *m_globalObject;
    QString m_identifier;
};

QT_END_NAMESPACE

#endif // QIFSIMULATIONENGINE_H
