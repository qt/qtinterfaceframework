// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSERVICEMANAGER_H
#define QIFSERVICEMANAGER_H

#include <QtCore/QAbstractListModel>
#include <QtQml/QQmlEngine>

#include <QtInterfaceFramework/qtifglobal.h>
#include <QtInterfaceFramework/QIfServiceObject>

QT_BEGIN_NAMESPACE

class QIfServiceObject;
class QIfServiceManagerPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceManager : public QAbstractListModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ServiceManager)
    QML_SINGLETON

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
    static QIfServiceManager *create(QQmlEngine *, QJSEngine *);

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
