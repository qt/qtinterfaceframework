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

#ifndef QIFSERVICEMANAGER_H
#define QIFSERVICEMANAGER_H

#include <QtCore/QAbstractListModel>

#include <QtInterfaceFramework/qtifglobal.h>
#include <QtInterfaceFramework/QIfServiceObject>

QT_BEGIN_NAMESPACE

class QIfServiceObject;
class QIfServiceManagerPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceManager : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        ServiceObjectRole = Qt::UserRole,
        InterfacesRole = Qt::UserRole +1
    };

    enum SearchFlag {
        IncludeProductionBackends = 0x01,
        IncludeSimulationBackends = 0x02,
        IncludeAll = IncludeProductionBackends | IncludeSimulationBackends,
    };
    Q_DECLARE_FLAGS(SearchFlags, SearchFlag)
    Q_FLAG(SearchFlags)

    enum BackendType {
        ProductionBackend,
        SimulationBackend
    };
    Q_ENUM(BackendType)

    static QIfServiceManager *instance();

    Q_INVOKABLE QList<QIfServiceObject*> findServiceByInterface(const QString &interface, QIfServiceManager::SearchFlags searchFlags = IncludeAll);
    Q_INVOKABLE bool hasInterface(const QString &interface) const;

    bool registerService(QObject *serviceBackendInterface, const QStringList &interfaces, QIfServiceManager::BackendType backendType = ProductionBackend);
    void unloadAllBackends();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    explicit QIfServiceManager();
    QIfServiceManagerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QIfServiceManager)
};

QT_END_NAMESPACE

#endif // QIFSERVICEMANAGER_H
