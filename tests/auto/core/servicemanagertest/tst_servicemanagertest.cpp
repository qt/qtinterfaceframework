// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QString>
#include <QtTest>
#include <QQmlEngine>
#include <QQmlComponent>
#include <qifservicemanager.h>
#include <QtInterfaceFramework/private/qifservicemanager_p.h>
#include <qifserviceinterface.h>
#include <qifserviceobject.h>
#include <QIfProxyServiceObject>
#include <private/qifproxyserviceobject_p.h>

class MockServiceBackend : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIfServiceInterface)

public:
    MockServiceBackend(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    MockServiceBackend(const QString& id, QObject *parent = nullptr)
        : QObject(parent)
        , m_id(id)
    {
    }

    QString id() const override {
        return m_id;
    }

    QStringList interfaces() const override {
        return m_serviceObjects.keys();
    }

    QIfFeatureInterface *interfaceInstance(const QString &interface) const override {
        return m_serviceObjects.value(interface);
    }

    void addServiceObject(const QString &interface, QIfFeatureInterface *serviceObject) {
        if (!serviceObject->parent())
            serviceObject->setParent(this);

        m_serviceObjects.insert(interface, serviceObject);
    }

private:
    QMap<QString, QIfFeatureInterface *> m_serviceObjects;
    QString m_id;
};

class TestInterface : public QIfFeatureInterface
{
    Q_OBJECT
public:
    TestInterface(QObject *parent)
        : QIfFeatureInterface(parent)
    {}

    void initialize() override
    {
    }
};

class ServiceManagerTest : public QObject
{
    Q_OBJECT

public:
    ServiceManagerTest();

    void ignoreStaticPluginWarnings();
    void ignoreDynamicPluginWarnings();

private Q_SLOTS:
    void initTestCase();
    void cleanup();

    void testRetakeSingleton();
    void testHasInterface();
    void testInvalidServiceHandles();
    void testFindServiceObjectsReturnInValidInstance();
    void testFindServiceObjects_data();
    void testFindServiceObjects();
    void testLoadServiceObjects_data();
    void testLoadServiceObjects();
    void testLoadServiceObjectsQML_data();
    void testLoadServiceObjectsQML();
    void testSyncAfterAsyncLoading();
    void testPreferredBackends();
    void testRegisterWithNoInterfaces();
    void testRegisterNonServiceBackendInterfaceObject();
    void testManagerListModel();
    void pluginLoaderTest();

private:
    QIfServiceManager *manager;
    QString m_simplePluginID;
};

ServiceManagerTest::ServiceManagerTest()
{
}

void ServiceManagerTest::ignoreStaticPluginWarnings()
{
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("PluginManager - Malformed metaData in static plugin '.*'. MetaData must contain a list of interfaces"));
}

void ServiceManagerTest::ignoreDynamicPluginWarnings()
{
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("PluginManager - Malformed metaData in '.*'. MetaData must contain a list of interfaces"));
#ifdef DEBUG_AND_RELEASE
# ifndef Q_OS_WIN
    // Because the plugin is build in both configurations, the error is also emitted twice
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("PluginManager - Malformed metaData in '.*'. MetaData must contain a list of interfaces"));
# endif

    // FIXME Disabled for cmake porting
    // QTest::ignoreMessage(QtInfoMsg, QRegularExpression("Found the same plugin in two configurations. Using the '.*' configuration: .*"));
#endif
}

void ServiceManagerTest::initTestCase()
{
    QStringList defaultLibraryPath = QCoreApplication::libraryPaths();
    // Make sure the dynamic plugins can't be found in the beginning
    QCoreApplication::setLibraryPaths(QStringList());
    ignoreStaticPluginWarnings();
    manager = QIfServiceManager::instance();

    QList<QIfServiceObject *> services = manager->findServiceByInterface("simple_plugin");
    QCOMPARE(services.count(), 0);

    // Unload all plugins and don't search for the static plugins to trigger the 'no plugins found' warning
    manager->unloadAllBackends();
    QIfServiceManagerPrivate::get(manager)->m_staticLoaded = true;

    QTest::ignoreMessage(QtWarningMsg, "No plugins found in search path:  \"\"");
    services = manager->findServiceByInterface("simple_plugin_static");
    QCOMPARE(services.count(), 0);

#ifdef Q_OS_ANDROID
    // On android we need to keep the default path as libraries can only be loaded from there
    QCoreApplication::setLibraryPaths(defaultLibraryPath);
#else
    // Change the library path to the current directory to be able to test loading dynamic plugins
    QCoreApplication::setLibraryPaths({QDir::currentPath()});
#endif
    ignoreDynamicPluginWarnings();

    // This needs to trigger a search for new plugins in the library path, as it is supposed to
    // reevaluate QCoreApplication::libraryPaths();
    services = manager->findServiceByInterface("simple_plugin");
    QCOMPARE(services.count(), 1);
    // Save the id of the service object. This needed in the pluginLoaderTest
    m_simplePluginID = services.at(0)->id();

    cleanup();
}

void ServiceManagerTest::cleanup()
{
    manager->unloadAllBackends();

    // Make sure to search for all plugins here explicitly to catch all expected warning messages
    // Otherwise a findServiceByInterface call will implictly trigger it.
    ignoreStaticPluginWarnings();
    ignoreDynamicPluginWarnings();
    QIfServiceManagerPrivate::get(manager)->searchPlugins();
}

void ServiceManagerTest::testRetakeSingleton()
{
    QPointer<QIfServiceManager> serviceManager = QIfServiceManager::instance();
    QQmlEngine *engine = new QQmlEngine;

    QByteArray qml ("import QtQuick; \n\
                     import QtInterfaceFramework; \n\
                     QtObject { \n\
                         Component.onCompleted: { \n\
                             var count = ServiceManager.count; \n\
                         } \n\
                     } \n\
                    ");
    QQmlComponent component(engine);
    component.setData(qml, QUrl());
    QScopedPointer<QObject> obj(component.create());
    QVERIFY2(obj, qPrintable(component.errorString()));

    delete engine;

    QVERIFY(!serviceManager.isNull());
}

#define COMPARE_SERVICE_OBJECT(_model_, _index_, _serviceObject_) \
do { \
    QString name = _model_->data(_model_->index(_index_), Qt::DisplayRole).toString(); \
    QCOMPARE(name, _serviceObject_->name()); \
\
    QStringList list = _model_->data(_model_->index(_index_), ServiceObjectModel::InterfacesRole).toStringList(); \
    QCOMPARE(list, _serviceObject_->interfaces()); \
\
    ServiceObject *so = _model_->data(_model_->index(_index_), ServiceObjectModel::ServiceObjectRole).value<ServiceObject*>(); \
    QCOMPARE(so, _serviceObject_); \
} while (0)

/*
    Test the hasInterface method
*/
void ServiceManagerTest::testHasInterface()
{
    QCOMPARE(manager->hasInterface("Foo"), false);

    MockServiceBackend *backend0 = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend0, QStringList() << "Foo" << "Bar");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->hasInterface("Foo"), true);
    QCOMPARE(manager->hasInterface("Bar"), true);
}

void ServiceManagerTest::testInvalidServiceHandles()
{
    QIfServiceObjectHandle handle;
    QCOMPARE(handle.isValid(), false);
    QCOMPARE(handle.isLoaded(), false);
    QCOMPARE(handle.serviceObject(), nullptr);
}

void ServiceManagerTest::testFindServiceObjectsReturnInValidInstance()
{
    QList<QIfServiceObject*> list = manager->findServiceByInterface("NonExistingInterface");
    QVERIFY(list.isEmpty());

    QList<QIfServiceObjectHandle> handles = manager->findServiceHandleByInterface("NonExistingInterface");
    QVERIFY(handles.isEmpty());
}

void ServiceManagerTest::testFindServiceObjects_data()
{
    QTest::addColumn<QIfServiceManager::SearchFlags>("searchFlags");
    QTest::newRow("AllBackends") << QIfServiceManager::SearchFlags(QIfServiceManager::IncludeAll);
    QTest::newRow("OnlyProductionBackends") << QIfServiceManager::SearchFlags(QIfServiceManager::IncludeProductionBackends);
    QTest::newRow("OnlySimulationBackends") << QIfServiceManager::SearchFlags(QIfServiceManager::IncludeSimulationBackends);
}

void ServiceManagerTest::testFindServiceObjects()
{
    QFETCH(QIfServiceManager::SearchFlags, searchFlags);
    MockServiceBackend *backend = new MockServiceBackend(manager);
    QIfServiceManager::BackendType type = QIfServiceManager::ProductionBackend;
    if (searchFlags & QIfServiceManager::IncludeSimulationBackends)
        type = QIfServiceManager::SimulationBackend;
    bool regResult = manager->registerService(backend, QStringList() << "TestInterface", type);
    QCOMPARE(regResult, true);
    QIfFeatureInterface *testObject = new TestInterface(backend);
    backend->addServiceObject("TestInterface", testObject);

    // Search using handles
    // Make sure that the handles are valid, but not loaded yet
    QList<QIfServiceObjectHandle> handles = manager->findServiceHandleByInterface("TestInterface", searchFlags);
    QCOMPARE(handles.count(), 1);
    QCOMPARE(handles.at(0).isValid(), true);
    QCOMPARE(handles.at(0).isLoaded(), false);

    QList<QIfServiceObject*> list = manager->findServiceByInterface("TestInterface", searchFlags);
    QVERIFY(!list.isEmpty());
    QIfServiceObject *serviceObject = list.at(0);
    QVERIFY(serviceObject->interfaces().contains("TestInterface"));
    QCOMPARE(serviceObject->interfaceInstance("TestInterface"), testObject);

    // Search using handles again
    // Make sure that the handles are valid and is already loaded
    // The service object should be the same as the one we got before
    handles = manager->findServiceHandleByInterface("TestInterface", searchFlags);
    QCOMPARE(handles.count(), 1);
    QCOMPARE(handles.at(0).isValid(), true);
    QCOMPARE(handles.at(0).isLoaded(), true);
    QCOMPARE(handles.at(0).serviceObject(), serviceObject);
}

void ServiceManagerTest::testLoadServiceObjects_data()
{
    QTest::addColumn<bool>("asyncLoading");
    QTest::addColumn<QString>("interface");
    QTest::newRow("registered service sync") << false << "TestInterface";
    QTest::newRow("registered service async") << true << "TestInterface";
    QTest::newRow("dynamic plugin sync") << false << "simple_plugin";
    QTest::newRow("dynamic plugin async") << true << "simple_plugin";
    QTest::newRow("static plugin sync") << false << "simple_plugin_static";
    QTest::newRow("static plugin async") << true << "simple_plugin_static";
}

void ServiceManagerTest::testLoadServiceObjects()
{
    QFETCH(bool, asyncLoading);
    QFETCH(QString, interface);

    if (interface == "TestInterface") {
        MockServiceBackend *backend = new MockServiceBackend(manager);
        QIfFeatureInterface *testObject = new TestInterface(backend);
        backend->addServiceObject("TestInterface", testObject);
        QVERIFY(manager->registerService(backend, QStringList() << "TestInterface", QIfServiceManager::ProductionBackend));
    }

    QList<QIfServiceObjectHandle> handles = manager->findServiceHandleByInterface(interface);
    QCOMPARE(handles.count(), 1);
    QCOMPARE(handles.at(0).isValid(), true);
    QCOMPARE(handles.at(0).isLoaded(), false);

    // Load the service object
    QSignalSpy serviceObjectLoadedSpy(manager, &QIfServiceManager::serviceObjectLoaded);
    manager->loadServiceObject(handles.at(0), asyncLoading);
    // Make sure the signal is emitted immediately when loading synchronously
    if (asyncLoading) {
        QCOMPARE(serviceObjectLoadedSpy.count(), 0);

        // Make sure that calling manager->loadServiceObject with the same handle multiple times
        // Doesn't cause the signal to be emitted multiple times
        manager->loadServiceObject(handles.at(0), asyncLoading);

        serviceObjectLoadedSpy.wait();
        QCOMPARE(serviceObjectLoadedSpy.count(), 1);
    } else {
        QCOMPARE(serviceObjectLoadedSpy.count(), 1);
    }
    QCOMPARE(serviceObjectLoadedSpy.at(0).count(), 1);
    QCOMPARE(serviceObjectLoadedSpy.at(0).at(0).value<QIfServiceObjectHandle>(), handles.at(0));
    QCOMPARE(handles.at(0).isValid(), true);
    QCOMPARE(handles.at(0).isLoaded(), true);
    QCOMPARE(handles.at(0).serviceObject()->interfaces().contains(interface), true);

    // Calling loadServiceObject again should cause the signal to be emitted again
    serviceObjectLoadedSpy.clear();
    manager->loadServiceObject(handles.at(0), false);
    QCOMPARE(serviceObjectLoadedSpy.count(), 1);
    QCOMPARE(serviceObjectLoadedSpy.at(0).count(), 1);
    QCOMPARE(serviceObjectLoadedSpy.at(0).at(0).value<QIfServiceObjectHandle>(), handles.at(0));
    QCOMPARE(handles.at(0).isValid(), true);
    QCOMPARE(handles.at(0).isLoaded(), true);
    QCOMPARE(handles.at(0).serviceObject()->interfaces().contains(interface), true);
}

void ServiceManagerTest::testLoadServiceObjectsQML_data()
{
    QTest::addColumn<bool>("asyncLoading");
    QTest::addColumn<QString>("interface");
    QTest::newRow("registered service sync") << false << "TestInterface";
    QTest::newRow("registered service async") << true << "TestInterface";
    QTest::newRow("dynamic plugin sync") << false << "simple_plugin";
    QTest::newRow("dynamic plugin async") << true << "simple_plugin";
    QTest::newRow("static plugin sync") << false << "simple_plugin_static";
    QTest::newRow("static plugin async") << true << "simple_plugin_static";
}

void ServiceManagerTest::testLoadServiceObjectsQML()
{
    QFETCH(bool, asyncLoading);
    QFETCH(QString, interface);

    if (interface == "TestInterface") {
        MockServiceBackend *backend = new MockServiceBackend(manager);
        QIfFeatureInterface *testObject = new TestInterface(backend);
        backend->addServiceObject("TestInterface", testObject);
        QVERIFY(manager->registerService(backend, QStringList() << "TestInterface", QIfServiceManager::ProductionBackend));
    }

    QQmlEngine engine;
    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:/testdata/loadserviceobject.qml")));
    QObject *obj = component.create();
    QVERIFY2(obj, qPrintable(component.errorString()));

    QSignalSpy loadingSucceededSpy(obj, SIGNAL(loadingSucceeded()));
    QVERIFY(loadingSucceededSpy.isValid());
    QVERIFY(QMetaObject::invokeMethod(obj, "findAndLoad", Q_ARG(QString, interface), Q_ARG(bool, asyncLoading)));
    if (asyncLoading)
        loadingSucceededSpy.wait();
    QCOMPARE(loadingSucceededSpy.count(), 1);
    delete obj;
}

void ServiceManagerTest::testSyncAfterAsyncLoading()
{
    QList<QIfServiceObjectHandle> handles = manager->findServiceHandleByInterface("simple_plugin");
    QCOMPARE(handles.count(), 1);
    QCOMPARE(handles.at(0).isValid(), true);
    QCOMPARE(handles.at(0).isLoaded(), false);

    // Load the service object
    QSignalSpy serviceObjectLoadedSpy(manager, &QIfServiceManager::serviceObjectLoaded);
    manager->loadServiceObject(handles.at(0), true);
    QCOMPARE(serviceObjectLoadedSpy.count(), 0);

    // Wait one event loop cycle to make sure the async loading thread started
    QTest::qWait(0);
    QCOMPARE(serviceObjectLoadedSpy.count(), 0);

    // Start to load the same plugin synchronously
    manager->loadServiceObject(handles.at(0), false);

    // DOES IT MAKE SENSE TO HAVE IT EMITTED TWICE ?
    QCOMPARE(serviceObjectLoadedSpy.count(), 2);
    QCOMPARE(serviceObjectLoadedSpy.at(0).count(), 1);
    QCOMPARE(serviceObjectLoadedSpy.at(0).at(0).value<QIfServiceObjectHandle>(), handles.at(0));
    QCOMPARE(handles.at(0).isValid(), true);
    QCOMPARE(handles.at(0).isLoaded(), true);
    QCOMPARE(handles.at(0).serviceObject()->interfaces().contains("simple_plugin"), true);
}

void ServiceManagerTest::testPreferredBackends()
{
    MockServiceBackend *mock_backend = new MockServiceBackend("mock", manager);
    QVERIFY(manager->registerService(mock_backend, QStringList({"Foo"})));

    MockServiceBackend *qtro_backend = new MockServiceBackend("qtro", manager);
    QVERIFY(manager->registerService(qtro_backend, QStringList({"Foo"})));

    QList<QIfServiceObject*> foundBackends = manager->findServiceByInterface("Foo");
    QCOMPARE(foundBackends.count(), 2);

    foundBackends = manager->findServiceByInterface("Foo", QIfServiceManager::IncludeAll, QStringList({"mock"}));
    QCOMPARE(foundBackends.count(), 1);
    QCOMPARE(foundBackends.at(0)->id(), "mock");

    foundBackends = manager->findServiceByInterface("Foo", QIfServiceManager::IncludeAll, QStringList({"*no_such_backend*", "qtro"}));
    QCOMPARE(foundBackends.count(), 1);
    QCOMPARE(foundBackends.at(0)->id(), "qtro");

    foundBackends = manager->findServiceByInterface("Foo", QIfServiceManager::IncludeAll, QStringList({"*no_such_backend*"}));
    QCOMPARE(foundBackends.count(), 2);
}

/*
    Test that the registerService method returns false if the user tries
    to register a service with an empty list of interfaces.
*/
void ServiceManagerTest::testRegisterWithNoInterfaces()
{
    MockServiceBackend *backend = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend, QStringList());
    QCOMPARE(regResult, false);
}

/*
    Test that the registerService method returns false if the user tries
    to register a service which doesn't implement the ServiceBackendInterface.
*/
void ServiceManagerTest::testRegisterNonServiceBackendInterfaceObject()
{
    QObject *anyObject = new QObject(manager);
    bool regResult = manager->registerService(anyObject, QStringList() << "Interface");
    QCOMPARE(regResult, false);
    QCOMPARE(manager->hasInterface("Interface"), false);
}

/*
    Test typical QAbstractListModel behavior
*/
void ServiceManagerTest::testManagerListModel()
{
    // Because of the plugin loading test and the static plugins, we always have some backends already
    // in the list
    const int backendCount = manager->rowCount();

    QSignalSpy managerModelSpy(manager, SIGNAL(rowsInserted(QModelIndex,int,int)));

    // Sanity check
    QCOMPARE(manager->rowCount(), backendCount);
    QCOMPARE(managerModelSpy.count(), 0);
    QCOMPARE(manager->data(QModelIndex(), QIfServiceManager::NameRole), QVariant());
    QCOMPARE(manager->data(QModelIndex(), QIfServiceManager::ServiceObjectRole), QVariant());
    QCOMPARE(manager->data(QModelIndex(), QIfServiceManager::ServiceObjectHandleRole), QVariant());
    QCOMPARE(manager->data(QModelIndex(), QIfServiceManager::InterfacesRole), QVariant());

    // Register backend-0 with 'Interface0'
    MockServiceBackend *backend0 = new MockServiceBackend(manager);
    bool regResult = manager->registerService(backend0, QStringList() << "Interface0");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), backendCount + 1);
    //QCOMPARE(manager->data(manager->index(0), Qt::DisplayRole).value<QIfServiceInterface*>(), backend0);
    QCOMPARE(manager->data(manager->index(backendCount), QIfServiceManager::NameRole).toString(), QStringLiteral("MockServiceBackend"));
    QCOMPARE(manager->data(manager->index(backendCount), QIfServiceManager::ServiceObjectRole).value<QIfProxyServiceObject*>()->d_ptr->m_serviceInterface, backend0);
    auto handle = manager->data(manager->index(backendCount), QIfServiceManager::ServiceObjectHandleRole).value<QIfServiceObjectHandle>();
    QCOMPARE(handle.isValid(), true);
    QCOMPARE(handle.isLoaded(), true);
    QCOMPARE(static_cast<QIfProxyServiceObject*>(handle.serviceObject())->d_ptr->m_serviceInterface, backend0);
    QCOMPARE(manager->data(manager->index(backendCount), QIfServiceManager::InterfacesRole).toStringList(), QStringList() << "Interface0");
    QCOMPARE(managerModelSpy.count(), 1);
    // Extendend sanity check
    QCOMPARE(manager->data(manager->index(0,0), Qt::UserRole + 200), QVariant());

    // Register backend-1 with 'Interface1' and 'Interface2'. Should change the model
    MockServiceBackend *backend1 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend1, QStringList() << "Interface1" << "Interface2");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), backendCount + 2);
    QCOMPARE(manager->data(manager->index(backendCount), QIfServiceManager::NameRole).toString(), QStringLiteral("MockServiceBackend"));
    QCOMPARE(manager->data(manager->index(backendCount), QIfServiceManager::ServiceObjectRole).value<QIfProxyServiceObject*>()->d_ptr->m_serviceInterface, backend0);
    handle = manager->data(manager->index(backendCount), QIfServiceManager::ServiceObjectHandleRole).value<QIfServiceObjectHandle>();
    QCOMPARE(handle.isValid(), true);
    QCOMPARE(handle.isLoaded(), true);
    QCOMPARE(static_cast<QIfProxyServiceObject*>(handle.serviceObject())->d_ptr->m_serviceInterface, backend0);
    QCOMPARE(manager->data(manager->index(backendCount), QIfServiceManager::InterfacesRole).toStringList(), QStringList() << "Interface0");
    QCOMPARE(manager->data(manager->index(backendCount + 1), QIfServiceManager::NameRole).toString(), QStringLiteral("MockServiceBackend"));
    QCOMPARE(manager->data(manager->index(backendCount + 1), QIfServiceManager::ServiceObjectRole).value<QIfProxyServiceObject*>()->d_ptr->m_serviceInterface, backend1);
    handle = manager->data(manager->index(backendCount + 1), QIfServiceManager::ServiceObjectHandleRole).value<QIfServiceObjectHandle>();
    QCOMPARE(handle.isValid(), true);
    QCOMPARE(handle.isLoaded(), true);
    QCOMPARE(static_cast<QIfProxyServiceObject*>(handle.serviceObject())->d_ptr->m_serviceInterface, backend1);
    QCOMPARE(manager->data(manager->index(backendCount + 1), QIfServiceManager::InterfacesRole).toStringList(), QStringList() << "Interface1" << "Interface2");
    QCOMPARE(managerModelSpy.count(), 2);

    // Register backend-2 with 'Interface1' and 'Interface2'. Should not result in any model changes
    MockServiceBackend *backend2 = new MockServiceBackend(manager);
    regResult = manager->registerService(backend2, QStringList() << "Interface1" << "Interface2");
    QCOMPARE(regResult, true);
    QCOMPARE(manager->rowCount(), backendCount + 3);
    QCOMPARE(managerModelSpy.count(), 3);
}

void ServiceManagerTest::pluginLoaderTest()
{
    QVERIFY(manager->hasInterface("simple_plugin"));
    QList<QIfServiceObject *> services = manager->findServiceByInterface("simple_plugin", QIfServiceManager::IncludeProductionBackends);
    QCOMPARE(services.count(), 1);
    //Because we unloaded the backend and created a new instance of it we expect to get a different id for the ServiceObject as in initTestCase()
    QVERIFY(m_simplePluginID != services.at(0)->id());

    //Test whether the loading of static plugins works as well
    services = manager->findServiceByInterface("simple_plugin_static", QIfServiceManager::IncludeSimulationBackends);
    QCOMPARE(services.count(), 1);

    QVERIFY(manager->hasInterface("wrong_plugin"));
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("ServiceManager::serviceObjects - failed to cast to interface from '.*wrong.*'"));
    QList<QIfServiceObject *> wServices = manager->findServiceByInterface("wrong_plugin");
    QCOMPARE(wServices.count(), 0);

    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("ServiceManager::serviceObjects - failed to cast to interface from '.*wrong.*'"));
    auto handles = manager->findServiceHandleByInterface("wrong_plugin");
    QCOMPARE(handles.count(), 1);
    QSignalSpy serviceObjectLoadedSpy(manager, &QIfServiceManager::serviceObjectLoaded);
    manager->loadServiceObject(handles.at(0), true);
    QCOMPARE(serviceObjectLoadedSpy.count(), 0);
    serviceObjectLoadedSpy.wait();
    QCOMPARE(serviceObjectLoadedSpy.count(), 1);
    QCOMPARE(serviceObjectLoadedSpy.at(0).count(), 1);
    auto handle = serviceObjectLoadedSpy.at(0).at(0).value<QIfServiceObjectHandle>();
    QCOMPARE(handle, handles.at(0));
    QCOMPARE(handle.isValid(), true);
    QCOMPARE(handle.isLoaded(), false);
    QCOMPARE(handle.serviceObject(), nullptr);

    //Test that the plugin is unloaded (or at least removed from the registry)
    manager->unloadAllBackends();
    QCOMPARE(manager->rowCount(), 0);
}

QTEST_MAIN(ServiceManagerTest)

#include "tst_servicemanagertest.moc"
