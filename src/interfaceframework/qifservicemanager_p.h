// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSERVICEMANAGER_P_H
#define QIFSERVICEMANAGER_P_H

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

#include <QtCore/QAbstractListModel>
#include <QtCore/QLoggingCategory>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>

#include <QtInterfaceFramework/qifservicemanager.h>
#include <private/qtifglobal_p.h>

QT_BEGIN_NAMESPACE

class QIfServiceInterface;
class QIfServiceObject;
class QIfProxyServiceObject;

Q_DECLARE_LOGGING_CATEGORY(qLcIfServiceManagement)

struct Backend {
    Q_DISABLE_COPY_MOVE(Backend)

    Backend() = default;
    ~Backend();

    QString name;
    bool debug = false;
    QVariantMap metaData;
    QIfServiceInterface *interface = nullptr;
    QIfProxyServiceObject *proxyServiceObject = nullptr;
    QAtomicInteger<bool> loading = false;
    QPointer<QThread> thread = nullptr;
    QPluginLoader *loader = nullptr;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit QIfServiceManagerPrivate(QIfServiceManager *parent);

    static QIfServiceManagerPrivate* get(QIfServiceManager *serviceManager);
    static bool isSimulation(const QVariantMap &metaData);

    QIfProxyServiceObject *createServiceObject(struct Backend *backend) const;
    QList<QIfServiceObjectHandle> findServiceByInterface(const QString &interface, QIfServiceManager::SearchFlags searchFlags, const QStringList &preferredBackends) const;

    void searchPlugins();
    void registerStaticBackend(const QStaticPlugin &plugin);
    void registerBackend(const QString &fileName, const QJsonObject &metaData);
    bool registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces, QIfServiceManager::BackendType backendType);
    void addBackend(struct Backend *backend);

    void unloadAllBackends();

    QIfServiceInterface *loadServiceBackendInterface(struct Backend *backend) const;
    void loadServiceBackendInterfaceAsync(struct Backend *backend);
    static std::tuple<QIfServiceInterface *, QPluginLoader*> loadPlugin(const QString &pluginFile);

    Backend *verifyHandle(void *handle);

    QList<Backend*> m_backends;
    QSet<QString> m_interfaceNames;
    QStringList m_loadedPaths;
    bool m_staticLoaded;

    QIfServiceManager * const q_ptr;
    Q_DECLARE_PUBLIC(QIfServiceManager)

Q_SIGNALS:
    void beginInsertRows(const QModelIndex &index, int start, int end);
    void endInsertRows();
};

QT_END_NAMESPACE

#endif // QIFSERVICEMANAGER_P_H

