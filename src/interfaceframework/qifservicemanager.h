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

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceObjectHandle
{
    Q_GADGET
    QML_VALUE_TYPE(ServiceObjectHandle)
    QML_ADDED_IN_VERSION(6, 8)
    Q_PROPERTY(bool valid READ isValid FINAL)
    Q_PROPERTY(bool loaded READ isLoaded FINAL)
    Q_PROPERTY(QIfServiceObject *serviceObject READ serviceObject FINAL)

public:
    QIfServiceObjectHandle() = default;
    ~QIfServiceObjectHandle() = default;
    QIfServiceObjectHandle(const QIfServiceObjectHandle &) = default;

    bool isValid() const;
    bool isLoaded() const;
    QIfServiceObject *serviceObject() const;

    QIfServiceObjectHandle& operator=(const QIfServiceObjectHandle&) = default;
    bool operator==(QIfServiceObjectHandle other) const { return m_handle == other.m_handle; }
    bool operator!=(QIfServiceObjectHandle other) const { return !(*this == other); }

private:
    QIfServiceObjectHandle(void *handle);
    void *m_handle = nullptr;

    friend class QIfServiceManagerPrivate;
    friend class QIfServiceManager;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceManager : public QAbstractListModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(ServiceManager)
    QML_SINGLETON

public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        ServiceObjectRole = Qt::UserRole,
        InterfacesRole,
        ServiceObjectHandleRole,
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
    ~QIfServiceManager() override;

    Q_INVOKABLE QList<QIfServiceObject*> findServiceByInterface(const QString &interface, QIfServiceManager::SearchFlags searchFlags = IncludeAll, const QStringList &preferredBackends = QStringList());
    Q_REVISION(6, 8) Q_INVOKABLE QList<QIfServiceObjectHandle> findServiceHandleByInterface(const QString &interface, QIfServiceManager::SearchFlags searchFlags = IncludeAll, const QStringList &preferredBackends = QStringList());
    Q_INVOKABLE bool hasInterface(const QString &interface) const;

    bool registerService(QObject *serviceBackendInterface, const QStringList &interfaces, QIfServiceManager::BackendType backendType = ProductionBackend);
    void unloadAllBackends();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_REVISION(6, 8) Q_INVOKABLE void loadServiceObject(QIfServiceObjectHandle handle, bool async = false);

Q_SIGNALS:
    void serviceObjectLoaded(QIfServiceObjectHandle handle);

private:
    explicit QIfServiceManager(QObject *parent = nullptr);
    QIfServiceManagerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QIfServiceManager)

    friend class QIfServiceObjectHandle;
};

QT_END_NAMESPACE

#endif // QIFSERVICEMANAGER_H
