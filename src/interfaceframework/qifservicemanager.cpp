// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifservicemanager.h"

#include "qtinterfaceframeworkmodule.h"
#include "qifproxyserviceobject.h"
#include "qifservicemanager_p.h"
#include "qifconfiguration_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QLibrary>
#include <QModelIndex>
#include <QStringList>

using namespace Qt::StringLiterals;

#define QIF_PLUGIN_DIRECTORY "interfaceframework"

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLcIfServiceManagement, "qt.if.servicemanagement");

namespace qtif_helper {
#ifdef QT_DEBUG
    static const bool loadDebug = true;
#else
    static const bool loadDebug = false;
#endif
    static const QString interfacesLiteral = u"interfaces"_s;
    static const QString fileNameLiteral = u"fileName"_s;
    static const QString metaDataLiteral = u"MetaData"_s;
    static const QString classNameLiteral = u"className"_s;
    static const QString simulationLiteral = u"simulation"_s;
    static const QString debugLiteral = u"debug"_s;
#ifdef Q_OS_WIN
    static const QString debugSuffixLiteral = u"d"_s;
#else
    static const QString debugSuffixLiteral = u"_debug"_s;
#endif

    QString backendBaseName(const QString &fileName)
    {
        if (fileName.isEmpty())
            return fileName;
        const QFileInfo fi(fileName);
        //remove the library suffix
        QString baseName = fileName;
        baseName.chop(fi.suffix().size() + 1);

        //remove the configuration suffix
        if (baseName.endsWith(debugSuffixLiteral))
            baseName.chop(debugSuffixLiteral.size());

        return baseName;
    }
}

using namespace qtif_helper;

Backend::~Backend()
{
    delete interface;
    delete proxyServiceObject;
    delete loader;
}

QIfServiceManagerPrivate::QIfServiceManagerPrivate(QIfServiceManager *parent)
    : QObject(parent)
    , m_staticLoaded(false)
    , q_ptr(parent)
{
}

QIfServiceManagerPrivate *QIfServiceManagerPrivate::get(QIfServiceManager *serviceManager)
{
    Q_ASSERT(serviceManager);
    return serviceManager->d_ptr;
}

bool QIfServiceManagerPrivate::isSimulation(const QVariantMap &metaData)
{
    QString fileName = metaData[fileNameLiteral].toString();
    return fileName.contains(QLatin1String("_simulation")) ||
            fileName.contains(QLatin1String("_simulator")) ||
            metaData[simulationLiteral].toBool();
}

QIfProxyServiceObject *QIfServiceManagerPrivate::createServiceObject(struct Backend *backend) const
{
    if (!backend)
        return nullptr;

    if (!backend->proxyServiceObject) {
        QIfServiceInterface *backendInterface = loadServiceBackendInterface(backend);
        if (backendInterface)
            backend->proxyServiceObject = new QIfProxyServiceObject(backendInterface);
    }

    if (backend->proxyServiceObject) {
        QString fileName = backend->metaData[fileNameLiteral].toString();
        if (fileName.isEmpty())
            fileName = u"static plugin"_s;

        qCDebug(qLcIfServiceManagement) << "Found: " << backend->proxyServiceObject << "from: " << fileName;

        const QString configurationId = backend->proxyServiceObject->configurationId();
        if (!configurationId.isEmpty()) {
            // This will also apply the current serviceSettings to the serviceObject
            QIfConfigurationManager::instance()->addServiceObject(configurationId, backend->proxyServiceObject);
        }
        return backend->proxyServiceObject;
    }

    return nullptr;
}

QList<QIfServiceObject *> QIfServiceManagerPrivate::findServiceByInterface(const QString &interface, QIfServiceManager::SearchFlags searchFlags, const QStringList &preferredBackends) const
{
    QList<QIfServiceObject*> list;
    qCDebug(qLcIfServiceManagement) << "Searching for a backend for:" << interface << "SearchFlags:" << searchFlags << "PreferredBackends:" << preferredBackends;

    QList<Backend *> foundBackends;
    for (Backend *backend : m_backends) {

        if (backend->metaData[interfacesLiteral].toStringList().contains(interface)) {
            bool isSimulation = QIfServiceManagerPrivate::isSimulation(backend->metaData);
            if ((searchFlags & QIfServiceManager::IncludeSimulationBackends && isSimulation) ||
                (searchFlags & QIfServiceManager::IncludeProductionBackends && !isSimulation)) {
                foundBackends.append(backend);
            }
        }
    }

    // Filter the list ouf matching backends
    // The wildcards in the disambiguation list are checked in order
    // Once a wildcard founds matches those are returned.
    // In case of no match the next wildcard is used.
    for (const QString &wildCard : std::as_const(preferredBackends)) {
        qCDebug(qLcIfServiceManagement) << "Dissambiguate found backends with wildcard:" << wildCard;
        const auto regexp = QRegularExpression(QRegularExpression::wildcardToRegularExpression(wildCard));
        for (Backend *backend : std::as_const(foundBackends)) {
            const auto fileInfo = QFileInfo(backend->metaData[fileNameLiteral].toString());
            QIfServiceObject *serviceObject = nullptr;
            QString identifier = fileInfo.fileName();

            if (identifier.isEmpty() && backend->proxyServiceObject) {
                //static plugin
                identifier = backend->proxyServiceObject->id();
            }

            if (regexp.match(identifier).hasMatch())
                serviceObject = createServiceObject(backend);

            if (serviceObject)
                list.append(serviceObject);
            else
                qCDebug(qLcIfServiceManagement) << "Wildcard doesn't contain:" << identifier;
        }

        if (!list.isEmpty())
            return list;
    }

    if (list.isEmpty()) {
        qCDebug(qLcIfServiceManagement) << "Didn't find any preferred backends. Returning all found.";
        for (Backend *backend : std::as_const(foundBackends)) {
            auto serviceObject = createServiceObject(backend);
            if (serviceObject)
                list.append(serviceObject);
        }
    }

    return list;
}

void QIfServiceManagerPrivate::searchPlugins()
{
    bool found = false;

    const auto pluginDirs = QCoreApplication::libraryPaths();
    for (const QString &pluginDir : pluginDirs) {
        // Already loaded, skip it...
        if (m_loadedPaths.contains(pluginDir))
            continue;
        m_loadedPaths << pluginDir;

#ifdef Q_OS_ANDROID
        QString path = pluginDir;
#else
        QString path = pluginDir + QDir::separator() + QLatin1String(QIF_PLUGIN_DIRECTORY);
#endif
        QDir dir(path);
        //Check whether the directory exists
        if (!dir.exists())
            continue;

        const QStringList plugins = QDir(path).entryList(
#ifdef Q_OS_ANDROID
                    QStringList(QLatin1String("libplugins_%1_*.so").arg(QLatin1String(QIF_PLUGIN_DIRECTORY))),
#endif
                    QDir::Files);
        for (const QString &pluginFileName : plugins) {
            if (!QLibrary::isLibrary(pluginFileName))
                continue;

            const QFileInfo info(dir, pluginFileName);
            const QString absFile = info.canonicalFilePath();
            QPluginLoader loader(absFile);

            registerBackend(absFile, loader.metaData());
            found = true;
        }
    }

    // Only load the static plugins once
    if (!m_staticLoaded) {
        m_staticLoaded = true;
        const auto staticPlugins = QPluginLoader::staticPlugins();
        for (const QStaticPlugin &plugin : staticPlugins)
            registerStaticBackend(plugin);
    }

    if (Q_UNLIKELY(!found && m_backends.count() == 0))
        qWarning() << "No plugins found in search path: " << QCoreApplication::libraryPaths().join(QLatin1String(":"));
}

void QIfServiceManagerPrivate::registerBackend(const QString &fileName, const QJsonObject &metaData)
{
    QVariantMap backendMetaData = metaData.value(metaDataLiteral).toVariant().toMap();

    if (Q_UNLIKELY(backendMetaData[interfacesLiteral].isNull() ||
                   backendMetaData[interfacesLiteral].toList().isEmpty())) {
        qCWarning(qLcIfServiceManagement, "PluginManager - Malformed metaData in '%s'. MetaData must contain a list of interfaces", qPrintable(fileName));
        return;
    }

    backendMetaData.insert(fileNameLiteral, fileName);

    auto *backend = new Backend;
    backend->name =  metaData.value(classNameLiteral).toString();
    backend->debug = metaData.value(debugLiteral).toBool();
    backend->metaData = backendMetaData;
    addBackend(backend);
}

void QIfServiceManagerPrivate::registerStaticBackend(const QStaticPlugin &plugin)
{
    QVariantMap backendMetaData = plugin.metaData().value(metaDataLiteral).toVariant().toMap();
    const char* pluginName = plugin.instance()->metaObject()->className();

    if (Q_UNLIKELY(backendMetaData[interfacesLiteral].isNull() ||
                   backendMetaData[interfacesLiteral].toList().isEmpty())) {
        qCWarning(qLcIfServiceManagement, "PluginManager - Malformed metaData in static plugin '%s'. MetaData must contain a list of interfaces", pluginName);
        return;
    }

    QIfServiceInterface *backendInterface = qobject_cast<QIfServiceInterface*>(plugin.instance());
    if (Q_UNLIKELY(!backendInterface))
        qCWarning(qLcIfServiceManagement, "ServiceManager::serviceObjects - failed to cast to interface from '%s'", pluginName);

    //TODO check for other metaData like name etc.

    auto *backend = new Backend;
    backend->name = plugin.metaData().value(classNameLiteral).toString();
    backend->debug = plugin.metaData().value(debugLiteral).toBool();
    backend->metaData = backendMetaData;
    backend->interface = backendInterface;
    addBackend(backend);
}

bool QIfServiceManagerPrivate::registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces, QIfServiceManager::BackendType backendType)
{
    if (interfaces.isEmpty()) {
        return false;
    }

    // Verify that the object implements the ServiceBackendInterface
    QIfServiceInterface *interface = qobject_cast<QIfServiceInterface*>(serviceBackendInterface);
    if (!interface) {
        return false;
    }

    QVariantMap metaData = QVariantMap();

    metaData.insert(interfacesLiteral, interfaces);
    if (backendType == QIfServiceManager::SimulationBackend)
        metaData.insert(simulationLiteral, true);

    auto *backend = new Backend;
    backend->name = QString::fromLocal8Bit(serviceBackendInterface->metaObject()->className());
    backend->metaData = metaData;
    backend->interface = interface;

    addBackend(backend);
    return true;
}

void QIfServiceManagerPrivate::unloadAllBackends()
{
    Q_Q(QIfServiceManager);

    q->beginResetModel();
    for (Backend* backend : std::as_const(m_backends)) {
        if (backend->proxyServiceObject) {
            auto configurationId = backend->proxyServiceObject->configurationId();
            QIfConfigurationManager::instance()->removeServiceObject(configurationId, backend->proxyServiceObject);
        }

        delete backend;
    }
    m_backends.clear();
    q->endResetModel();

    m_interfaceNames.clear();
    m_loadedPaths.clear();
    m_staticLoaded = false;
}

void QIfServiceManagerPrivate::addBackend(Backend *backend)
{
    Q_Q(QIfServiceManager);
    //Check whether the same plugin is already in (maybe also in a different configuration)
    //The current configuration of QtInterfaceFramework decides which configuration takes precedence

    const QString newBackendFile = backend->metaData.value(fileNameLiteral).toString();
    const QString newBackendFileBase = qtif_helper::backendBaseName(newBackendFile);
    const QStringList ifaceList = backend->metaData.value(interfacesLiteral).toStringList();
    const QSet<QString> newInterfaces = QSet<QString>(ifaceList.begin(), ifaceList.end());

    bool addBackend = true;
    if (!newBackendFile.isEmpty()) {
        for (int i = 0; i < m_backends.count(); i++) {
            Backend *b = m_backends[i];
            const QStringList curIfaceList = backend->metaData.value(interfacesLiteral).toStringList();
            const QSet<QString> interfaces = QSet<QString>(curIfaceList.begin(), curIfaceList.end());
            if (interfaces == newInterfaces && b->name == backend->name) {
                const QString fileName = b->metaData.value(fileNameLiteral).toString();
                if (fileName == newBackendFile) {
                    qCDebug(qLcIfServiceManagement, "Skipping backend %s: already in the list", qPrintable(newBackendFile));
                    return;
                }

                QString base = backendBaseName(fileName);
                //check whether the plugins name are the same after removing the debug and library suffixes
                if (newBackendFileBase == base) {
                    qCInfo(qLcIfServiceManagement, "Found the same plugin in two configurations. "
                                                    "Using the '%s' configuration: %s",
                                                    qtif_helper::loadDebug ? "debug" : "release",
                                                    qPrintable(b->debug == qtif_helper::loadDebug ? fileName : newBackendFile));
                    if (b->debug != qtif_helper::loadDebug) {
                        qCDebug(qLcIfServiceManagement, "Replacing backend %s with %s", qPrintable(fileName), qPrintable(newBackendFile));
                        addBackend = false;
                        m_backends[i] = backend;
                        emit q->dataChanged(q->index(i, 0), q->index(i, 0));
                        delete b;
                        break;
                    } else {
                        qCDebug(qLcIfServiceManagement, "Skipping backend %s: wrong configuration", qPrintable(newBackendFile));
                        return;
                    }
                }
            }
        }
    }
    if (addBackend) {
        qCDebug(qLcIfServiceManagement, "Adding %s %s", qPrintable(newBackendFile.isEmpty() ? backend->name : newBackendFile),
                QIfServiceManagerPrivate::isSimulation(backend->metaData) ? "as simulation backend" : "as production backend");
        q->beginInsertRows(QModelIndex(), int(m_backends.count()), int(m_backends.count()));
        m_backends.append(backend);
        q->endInsertRows();
    }

    for (const QString &interface : newInterfaces)
        m_interfaceNames.insert(interface);
}

namespace {
Q_NEVER_INLINE
static QIfServiceInterface *warn(const char *what, const QPluginLoader *loader)
{
    qWarning("ServiceManager::serviceObjects - failed to %s '%s'",
             what, qPrintable(loader->fileName()));
    delete loader;
    return nullptr;
}
} // unnamed namespace

QIfServiceInterface *QIfServiceManagerPrivate::loadServiceBackendInterface(struct Backend *backend) const
{
    if (backend->interface) {
        return backend->interface;
    }

    QPluginLoader *loader = new QPluginLoader(backend->metaData[fileNameLiteral].toString());
    QObject *plugin = loader->instance();
    if (Q_UNLIKELY(!plugin))
        return warn("load", loader);

    QIfServiceInterface *backendInterface = qobject_cast<QIfServiceInterface*>(plugin);
    if (Q_UNLIKELY(!backendInterface))
        return warn("cast to interface from", loader);

    backend->interface = backendInterface;
    backend->loader = loader;
    return backend->interface;
}

/*!
    \class QIfServiceManager
    \inmodule QtInterfaceFramework
    \brief QIfServiceManager provides the backends to QIfAbstractFeature.

    QIfServiceManager is the heart of QtInterfaceFramework and provides you with an easy way to detect which
    backends and interfaces are available.

    By default, QIfServiceManager reads the metaData of all plugins within the \e interfaceframework folder
    in your plugin path. The plugin itself is loaded when you request for it explicitly, using
    findServiceByInterface().

    The manager can distinguish between \e Production and \e Simulation backends, using the
    filename or the metaData.

    Simulation Backends can set the \c simulation key in their plugin metadata to \c true or use
    one of two supported plugin postfixes (_simulation, _simulator) in the plugin filename.

    The registerService() function can be used to add Backend classes without putting them into
    a plugin.

    The service manager is a process-wide singleton and can be accessed through the \l instance
    method.

    If you require more detailed information on which plugins are recognized, make sure you enable
    the \c{qt.if.servicemanagement} logging category.

    For more information on how backends are loaded, see \l {Dynamic Backend System}.

    \note The QIfServiceManager only accepts plugins that match the build configuration used
    for building qtinterfaceframework. This means a qtinterfaceframework "release" build won't accept plugins from a
    "debug" build.
*/

/*!
    \enum QIfServiceManager::Roles

    \value NameRole
           The backend's name, such as MediaPlugin.
    \value ServiceObjectRole
           The actual QIfServiceObject, which can be used to connect a frontend API to this
           backend.
           \note When using this role in the data() function, the backend plugin is loaded and
           instantiated.
    \value InterfacesRole
           A list of interfaces that the backend implements.
*/

/*!
    \enum QIfServiceManager::SearchFlag

    \value IncludeProductionBackends
           Include production backends in the search result. \sa ProductionBackend
    \value IncludeSimulationBackends
           Include simulation backends in the search result. \sa SimulationBackend
    \value IncludeAll
           Include both production and simulation backends in the search result.
*/

/*!
    \enum QIfServiceManager::BackendType

    \value ProductionBackend
           A backend controlling a real automotive interface, such as a climate control connected
           over the CAN bus.
    \value SimulationBackend
           A backend used for development, that only returns simulation values and won't be
           deployed to the final hardware.
*/

/*!
    \qmltype ServiceManager
    \instantiates QIfServiceManager
    \inqmlmodule QtInterfaceFramework
    \brief The central instance that loads the backends and provides ServiceObjects.

    The ServiceManager singleton provides a model which can be used to list all available backends
    and their interfaces.

    This class can be useful when you want to query all available backends for a specific interface.
    For example, to show a list of all available backends which implement the MediaPlayer interface.
    The use can then be presented with an option to select between local playback, or playback using
    a bluetooth device.

    The ServiceManager implements the QAbstractListModel interface and provides the following roles:

    \table
    \header
        \li Role Name
        \li Type
        \li Description
    \row
        \li \c name
        \li string
        \li The backend's name, such as MediaPlugin.
    \row
        \li \c serviceObject
        \li ServiceObject
        \li The actual QIfServiceObject, which can be used to connect a frontend API to this backend.
            \note When using this role in the data() function, the backend plugin is loaded and
            instantiated.
    \row
        \li \c interfaces
        \li list<string>
        \li A list of interfaces implemented by the backend.
    \endtable

    For more information about QIfServiceManager and how it works, see its \l{QIfServiceManager}{C++ documentation}.
*/

QIfServiceManager::QIfServiceManager()
    : QAbstractListModel(nullptr)
    , d_ptr(new QIfServiceManagerPrivate(this))
{
    QtInterfaceFrameworkModule::registerTypes();
    d_ptr->searchPlugins();
}

/*!
    Returns the global service manager instance.
*/
QIfServiceManager *QIfServiceManager::instance()
{
    static auto *instance = new QIfServiceManager();
    return instance;
}

QIfServiceManager *QIfServiceManager::create(QQmlEngine *, QJSEngine *)
{
    auto manager = QIfServiceManager::instance();
    QQmlEngine::setObjectOwnership(manager, QQmlEngine::CppOwnership);
    return manager;
}

/*!
    \qmlmethod list<ServiceObject> ServiceManager::findServiceByInterface(interface, searchFlags, preferredBackends)

    Returns a list of backends implementing the specified \a interface.

    The \a searchFlags argument can be used to control which type of backends are included in the
    search result:

    \value IncludeProductionBackends
           Include production backends in the search result. See also \l {QIfServiceManager::}{ProductionBackend}
    \value IncludeSimulationBackends
           Include simulation backends in the search result. See also \l {QIfServiceManager::}{SimulationBackend}
    \value IncludeAll
           Include both production and simulation backends in the search result.

    The \a preferredBackends argument is used to select a backend when multiple backends implement
    the specified interface.
    The wildcards are applied in order to the found backends. If the wildcard matches some backends
    those backends will be loaded, otherwise the next wildcard is used.
*/
/*!
    Returns a list of backends implementing the specified \a interface.

    The \a searchFlags argument can be used to control which type of backends are included in the
    search result.

    The \a preferredBackends argument is used to select a backend when multiple backends implement
    the specified interface.
    The wildcards are applied in order to the found backends. If the wildcard matches some backends
    those backends will be loaded, otherwise the next wildcard is used.
*/
QList<QIfServiceObject *> QIfServiceManager::findServiceByInterface(const QString &interface, SearchFlags searchFlags, const QStringList &preferredBackends)
{
    Q_D(QIfServiceManager);
    d->searchPlugins();
    return d->findServiceByInterface(interface, searchFlags, preferredBackends);
}

/*!
    Registers a backend. The \a serviceBackendInterface specified must implement the
    QIfServiceInterface, otherwise the registration will fail. \a interfaces is a list of at least
    one interface, supported by the backend. The \a backendType indicates the type of the backend
    and influences whether the backend can be found by the Feature's auto discovery option.

    Returns \c true if the backend was successfully registered; otherwise \c false.

    \sa QIfServiceInterface
*/
bool QIfServiceManager::registerService(QObject *serviceBackendInterface, const QStringList &interfaces, BackendType backendType)
{
    Q_D(QIfServiceManager);
    return d->registerBackend(serviceBackendInterface, interfaces, backendType);
}

/*!
    \internal

    Unloads all backends currently loaded . Commonly only used for unit testing.
*/
void QIfServiceManager::unloadAllBackends()
{
    Q_D(QIfServiceManager);
    d->unloadAllBackends();
}

/*!
    \qmlmethod bool ServiceManager::hasInterface(interface)

    Returns \c true if the specified \a interface has been registered.
*/
/*!
    Returns \c true if the specified \a interface has been registered.
*/
bool QIfServiceManager::hasInterface(const QString &interface) const
{
    Q_D(const QIfServiceManager);
    return d->m_interfaceNames.contains(interface);
}

/*!
    Returns the number of rows for the given \a parent. Typically \a parent is an empty model
    index.

    \sa QAbstractListModel::data()
*/
int QIfServiceManager::rowCount(const QModelIndex &parent) const
{
    Q_D(const QIfServiceManager);
    return parent.isValid() ? 0 : int(d->m_backends.count());
}

/*!
    Returns the data for the \a index and \a role specified.

    \sa QAbstractListModel::data()
*/
QVariant QIfServiceManager::data(const QModelIndex &index, int role) const
{
    Q_D(const QIfServiceManager);

    if (!index.isValid())
        return QVariant();

    int row = index.row();

    if (row < 0 || row >= d->m_backends.count())
        return QVariant();

    Backend *backend = d->m_backends.at(row);

    switch (role) {
    case NameRole: return backend->name;
    case ServiceObjectRole: return QVariant::fromValue(d->createServiceObject(backend));
    case InterfacesRole: return backend->metaData[interfacesLiteral];
    }

    return QVariant();
}


/*!
    \reimp
*/
QHash<int, QByteArray> QIfServiceManager::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[NameRole] = "name";
        roles[ServiceObjectRole] = "serviceObject";
        roles[InterfacesRole] = "interfaces";
    }
    return roles;
}

QT_END_NAMESPACE

#include "moc_qifservicemanager.cpp"
