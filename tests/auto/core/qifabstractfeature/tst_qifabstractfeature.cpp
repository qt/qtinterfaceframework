// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0


#include <QtTest>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QIfServiceObject>
#include <QIfProxyServiceObject>
#include <QIfServiceInterface>
#include <QIfAbstractFeatureListModel>
#include <QIfServiceManager>

#include "qiffeaturetester.h"

int acceptCounter = 100;

class TestFeatureInterface : public QIfFeatureInterface
{
    Q_OBJECT

public:
    explicit TestFeatureInterface(QObject *parent = nullptr)
        : QIfFeatureInterface(parent)
    {}

    ~TestFeatureInterface() {}

Q_SIGNALS:
    void errorChanged(QIfAbstractFeature::Error error, const QString &message = QString());
};

class TestFeature : public QIfAbstractFeature
{
    Q_OBJECT

public:
    TestFeature(bool testBaseFunctions = false, QObject *parent = nullptr)
        : QIfAbstractFeature("testFeature", parent)
        , m_testBaseFunctions(testBaseFunctions)
    {}

    QString errorText() const
    {
        return QIfAbstractFeature::errorText();
    }

    virtual bool acceptServiceObject(QIfServiceObject *serviceObject) override
    {
        if (m_testBaseFunctions)
            return QIfAbstractFeature::acceptServiceObject(serviceObject);

        if (serviceObject && acceptCounter > 0)
            return serviceObject->interfaces().contains(interfaceName());
        acceptCounter++;
        return false;
    }

    virtual void connectToServiceObject(QIfServiceObject *serviceObject) override
    {
        if (!serviceObject)
            return;
        TestFeatureInterface* testInterface = qobject_cast<TestFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));
        connect(testInterface, &TestFeatureInterface::errorChanged, this, &TestFeature::onErrorChanged);

        QIfAbstractFeature::connectToServiceObject(serviceObject);
    }

    virtual void clearServiceObject() override
    {
    }

private:
    bool m_testBaseFunctions;
};

class TestFeatureListModel : public QIfAbstractFeatureListModel
{
    Q_OBJECT

public:
    TestFeatureListModel(bool testBaseFunctions = false, QObject *parent = nullptr)
        : QIfAbstractFeatureListModel("testFeature", parent)
        , m_testBaseFunctions(testBaseFunctions)
    {}

    QString errorText() const
    {
        return QIfAbstractFeatureListModel::errorText();
    }

    virtual bool acceptServiceObject(QIfServiceObject *serviceObject) override
    {
        if (m_testBaseFunctions)
            return QIfAbstractFeatureListModel::acceptServiceObject(serviceObject);

        if (serviceObject && acceptCounter > 0)
            return serviceObject->interfaces().contains(interfaceName());
        acceptCounter++;
        return false;
    }

    virtual void connectToServiceObject(QIfServiceObject *serviceObject) override
    {
        if (!serviceObject)
            return;
        TestFeatureInterface* testInterface = qobject_cast<TestFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));
        connect(testInterface, &TestFeatureInterface::errorChanged, this, &TestFeatureListModel::onErrorChanged);

        QIfAbstractFeatureListModel::connectToServiceObject(serviceObject);
    }

    virtual void clearServiceObject() override
    {
    }

    virtual int rowCount(const QModelIndex &parent) const override
    {
        Q_UNUSED(parent)
        return 0;
    }

    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        Q_UNUSED(index)
        Q_UNUSED(role)
        return QVariant();
    }

private:
    bool m_testBaseFunctions;
};

class TestFeatureBackend : public TestFeatureInterface
{
    Q_OBJECT

public:
    TestFeatureBackend(QObject *parent = nullptr)
        : TestFeatureInterface(parent)
    {}

    void initialize() override
    {
        emit initializationDone();
    }

    void emitError(QIfAbstractFeature::Error error, const QString &message)
    {
        emit errorChanged(error, message);
    }
};

class TestBackend : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIfServiceInterface)

public:
    TestBackend(QObject *parent = nullptr)
        : QObject(parent)
        , m_testBackend(new TestFeatureBackend(this))
    {}

    QStringList interfaces() const override
    {
        return QStringList(QString("testFeature"));
    }

    QIfFeatureInterface* interfaceInstance(const QString& interface) const override
    {
        if (interface == "testFeature")
            return m_testBackend;

        return 0;
    }

    void emitError(QIfAbstractFeature::Error error, const QString &message)
    {
        m_testBackend->emitError(error, message);
    }

private:
    TestFeatureBackend* m_testBackend;
};

class BaseTest : public QObject
{
    Q_OBJECT

public:
    BaseTest(bool testModel = false)
        : QObject()
        , m_manager(QIfServiceManager::instance())
        , m_isModel(testModel)
    {
    }

private Q_SLOTS:
    void cleanup();

    void testAutoDiscoveryFailure();
    void testAutoDiscoveryWithMultipleBackends_data();
    void testAutoDiscoveryWithMultipleBackends();
    void testAutoDiscovery_data();
    void testAutoDiscovery();
    void testAutoDiscovery_qml();
    void testProxyServiceObject();
    void testErrors_data();
    void testErrors();
    void testServiceObjectDestruction();
    void testResetServiceObject();

private:
    QIfFeatureTester *createTester(bool testBaseFunctions = false)
    {
        if (m_isModel)
            return new QIfFeatureTester(new TestFeatureListModel(testBaseFunctions, this), this);
        else
            return new QIfFeatureTester(new TestFeature(testBaseFunctions, this), this);
    }

    QIfServiceManager *m_manager;
    bool m_isModel;
};

void BaseTest::cleanup()
{
    m_manager->unloadAllBackends();
}

void BaseTest::testAutoDiscoveryFailure()
{
    QIfFeatureTester *f = createTester();
    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "There is no simulation backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    QIfAbstractFeature::DiscoveryResult result = f->startAutoDiscovery();
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());
    QCOMPARE(result, QIfAbstractFeature::ErrorWhileLoading);

    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    f->setDiscoveryMode(QIfAbstractFeature::LoadOnlyProductionBackends);
    result = f->startAutoDiscovery();
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());
    QCOMPARE(result, QIfAbstractFeature::ErrorWhileLoading);

    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces());

    acceptCounter = 0;
    QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    QTest::ignoreMessage(QtWarningMsg, "No suitable ServiceObject found.");
    result = f->startAutoDiscovery();
    QVERIFY(!f->serviceObject());
    QCOMPARE(result, QIfAbstractFeature::ErrorWhileLoading);
    acceptCounter = 100;

    auto list = m_manager->findServiceByInterface("testFeature");
    f->setServiceObject(list.at(0));
    result = f->startAutoDiscovery();
    QCOMPARE(result, QIfAbstractFeature::NoResult);
    f->setServiceObject(nullptr);

    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces());

    QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\". Using the first "
                                       "one (enable \"qt.if.servicemanagement\" logging to see which are found)");
    result = f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QCOMPARE(result, QIfAbstractFeature::ProductionBackendLoaded);
}

void BaseTest::testAutoDiscoveryWithMultipleBackends_data()
{
    QTest::addColumn<QIfServiceManager::BackendType>("firstBackendType");
    QTest::addColumn<QIfServiceManager::BackendType>("secondBackendType");
    QTest::addColumn<QIfAbstractFeature::DiscoveryResult>("result");
    QTest::newRow("Production") << QIfServiceManager::ProductionBackend << QIfServiceManager::ProductionBackend << QIfAbstractFeature::ProductionBackendLoaded;
    QTest::newRow("Simulation") << QIfServiceManager::SimulationBackend << QIfServiceManager::SimulationBackend << QIfAbstractFeature::SimulationBackendLoaded;
    //Fallback to Simulation if Production was not accepted
    QTest::newRow("Production - Simulation") << QIfServiceManager::ProductionBackend << QIfServiceManager::SimulationBackend << QIfAbstractFeature::SimulationBackendLoaded;
}

void BaseTest::testAutoDiscoveryWithMultipleBackends()
{
    QFETCH(QIfServiceManager::BackendType, firstBackendType);
    QFETCH(QIfServiceManager::BackendType, secondBackendType);
    QFETCH(QIfAbstractFeature::DiscoveryResult, result);
    TestBackend* backend1 = new TestBackend();
    m_manager->registerService(backend1, backend1->interfaces(), firstBackendType);
    TestBackend* backend2 = new TestBackend();
    m_manager->registerService(backend2, backend2->interfaces(), secondBackendType);

    //The first backend is not accepted, test that the second backend is tested as well and accepted now.
    QIfFeatureTester *f = createTester();
    acceptCounter = 0;
    if (firstBackendType == secondBackendType) {
        QTest::ignoreMessage(QtWarningMsg, "There is more than one backend implementing \"testFeature\". Using the first "
                                           "one (enable \"qt.if.servicemanagement\" logging to see which are found)");
    }
    if (secondBackendType != QIfServiceManager::ProductionBackend)
        QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QTest::ignoreMessage(QtWarningMsg, "ServiceObject is not accepted");
    QIfAbstractFeature::DiscoveryResult res = f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QVERIFY(f->isValid());
    QVERIFY(f->isInitialized());
    QCOMPARE(res, result);
}

void BaseTest::testAutoDiscovery_data()
{
    QTest::addColumn<QIfAbstractFeature::DiscoveryMode>("mode");
    QTest::addColumn<QIfAbstractFeature::DiscoveryResult>("result");
    QTest::addColumn<bool>("registerProduction");
    QTest::addColumn<bool>("testBaseFunctions");

    QTest::newRow("Production") << QIfAbstractFeature::LoadOnlyProductionBackends << QIfAbstractFeature::ProductionBackendLoaded << true << false;
    QTest::newRow("Simulation") << QIfAbstractFeature::LoadOnlySimulationBackends << QIfAbstractFeature::SimulationBackendLoaded << true << false;
    QTest::newRow("Auto") << QIfAbstractFeature::AutoDiscovery << QIfAbstractFeature::ProductionBackendLoaded << true << false;
    QTest::newRow("Auto Base Functions") << QIfAbstractFeature::AutoDiscovery << QIfAbstractFeature::ProductionBackendLoaded << true << true;
    QTest::newRow("Auto fallback") << QIfAbstractFeature::AutoDiscovery << QIfAbstractFeature::SimulationBackendLoaded << false << false;
}

void BaseTest::testAutoDiscovery()
{
    QFETCH(QIfAbstractFeature::DiscoveryMode, mode);
    QFETCH(QIfAbstractFeature::DiscoveryResult, result);
    QFETCH(bool, registerProduction);
    QFETCH(bool, testBaseFunctions);

    TestBackend* backend = new TestBackend();
    if (mode == QIfAbstractFeature::LoadOnlySimulationBackends || !registerProduction) {
        m_manager->registerService(backend, backend->interfaces(), QIfServiceManager::SimulationBackend);
    } else if (mode == QIfAbstractFeature::LoadOnlyProductionBackends) {
        m_manager->registerService(backend, backend->interfaces());
    } else {
        m_manager->registerService(backend, backend->interfaces());
        TestBackend* backend2 = new TestBackend();
        m_manager->registerService(backend2, backend2->interfaces(), QIfServiceManager::SimulationBackend);
    }
    QIfFeatureTester *f = createTester(testBaseFunctions);
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    f->setDiscoveryMode(mode);
    QSignalSpy validSpy(f, &QIfFeatureTester::isValidChanged);
    QSignalSpy initializedSpy(f, &QIfFeatureTester::isInitializedChanged);
    if (!registerProduction)
        QTest::ignoreMessage(QtWarningMsg, "There is no production backend implementing \"testFeature\" .");
    QIfAbstractFeature::DiscoveryResult res = f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QVERIFY(f->isValid());
    QCOMPARE(f->discoveryMode(), mode);
    QCOMPARE(validSpy.count(), 1);
    QCOMPARE(validSpy.at(0).at(0).toBool(), true);
    QCOMPARE(res, result);
    QCOMPARE(f->discoveryResult(), result);
    QVERIFY(f->isInitialized());
    QCOMPARE(initializedSpy.count(), 1);
    QCOMPARE(initializedSpy.at(0).at(0).toBool(), true);
}

void BaseTest::testAutoDiscovery_qml()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QQmlEngine engine;
    if (m_isModel)
        qmlRegisterType<TestFeatureListModel>("testfeature", 1, 0, "TestFeature");
    else
        qmlRegisterType<TestFeature>("testfeature", 1, 0, "TestFeature");
    QQmlComponent component(&engine, QUrl::fromLocalFile(QFINDTESTDATA("testdata/autodiscovery.qml")));
    QObject *obj = component.create();

    QVERIFY2(obj, qPrintable(component.errorString()));
    QIfFeatureTester *defaultItem;
    if (m_isModel)
        defaultItem = new QIfFeatureTester(obj->findChild<TestFeatureListModel*>("default"), this);
    else
        defaultItem = new QIfFeatureTester(obj->findChild<TestFeature*>("default"), this);
    QVERIFY(defaultItem);
    QCOMPARE(defaultItem->discoveryMode(), QIfAbstractFeature::AutoDiscovery);
    QVERIFY(defaultItem->serviceObject());
    QCOMPARE(defaultItem->discoveryResult(), QIfAbstractFeature::ProductionBackendLoaded);

    QIfFeatureTester *autoDiscoveryDisabledItem;
    if (m_isModel)
        autoDiscoveryDisabledItem = new QIfFeatureTester(obj->findChild<TestFeatureListModel*>("autoDiscoveryDisabled"), this);
    else
        autoDiscoveryDisabledItem = new QIfFeatureTester(obj->findChild<TestFeature*>("autoDiscoveryDisabled"), this);
    QVERIFY(autoDiscoveryDisabledItem);
    QSignalSpy autoDiscoveryChanged(autoDiscoveryDisabledItem, &QIfFeatureTester::discoveryModeChanged);
    QSignalSpy serviceObjectChangedSpy(autoDiscoveryDisabledItem, &QIfFeatureTester::serviceObjectChanged);
    QCOMPARE(autoDiscoveryDisabledItem->discoveryMode(), QIfAbstractFeature::NoAutoDiscovery);
    QVERIFY(!autoDiscoveryDisabledItem->serviceObject());
    QCOMPARE(autoDiscoveryDisabledItem->discoveryResult(), QIfAbstractFeature::NoResult);

    QVERIFY(QMetaObject::invokeMethod(obj, "discover"));
    QCOMPARE(autoDiscoveryChanged.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QCOMPARE(defaultItem->discoveryResult(), QIfAbstractFeature::ProductionBackendLoaded);

    delete defaultItem;
    delete autoDiscoveryDisabledItem;
    delete obj;
}

void BaseTest::testProxyServiceObject()
{
    TestBackend* backend = new TestBackend();

    QIfFeatureTester *f = createTester();
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());

    //Test setting a ProxyServiceObject using the QIfServiceInterface constructor
    QIfProxyServiceObject proxyObject(backend);

    f->setServiceObject(&proxyObject);
    QVERIFY(f->serviceObject());
    QVERIFY(f->isValid());
    QVERIFY(f->isInitialized());

    f->setServiceObject(nullptr);
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());

    //Test setting a ProxyServiceObject using the QHash constructor
    QString interface = backend->interfaces().at(0);
    QIfProxyServiceObject proxyObject2({{interface, backend->interfaceInstance(interface)}});

    f->setServiceObject(&proxyObject2);
    QVERIFY(f->serviceObject());
    QVERIFY(f->isValid());
    QVERIFY(f->isInitialized());

    f->setServiceObject(nullptr);
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());

    delete backend;
}

void BaseTest::testErrors_data()
{
    QTest::addColumn<QIfAbstractFeature::Error>("errorCode");
    QTest::addColumn<QString>("message");
    QTest::newRow("unknown") << QIfAbstractFeature::Unknown << "unknown";
    QTest::newRow("permission") << QIfAbstractFeature::PermissionDenied << "permission";
    QTest::newRow("invalid op") << QIfAbstractFeature::InvalidOperation << "invalid op";
    QTest::newRow("invalid zone") << QIfAbstractFeature::InvalidZone << "invalid zone";
    QTest::newRow("timeout") << QIfAbstractFeature::Timeout << "timeout";
}

void BaseTest::testErrors()
{
    QFETCH(QIfAbstractFeature::Error, errorCode);
    QFETCH(QString, message);

    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QIfFeatureTester *f = createTester();
    f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QSignalSpy errorChangedSpy(f, &QIfFeatureTester::errorChanged);

    backend->emitError(errorCode, message);
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f->error(), errorCode);
    QCOMPARE(f->errorMessage(), f->errorText() + " " + message);

    errorChangedSpy.clear();
    backend->emitError(QIfAbstractFeature::NoError, QString());
    QCOMPARE(errorChangedSpy.count(), 1);
    QCOMPARE(f->error(), QIfAbstractFeature::NoError);
    QCOMPARE(f->errorMessage(), QString());
}

void BaseTest::testServiceObjectDestruction()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QIfFeatureTester *f = createTester();
    QSignalSpy serviceObjectChangedSpy(f, &QIfFeatureTester::serviceObjectChanged);
    QSignalSpy initializedChangedSpy(f, &QIfFeatureTester::isInitializedChanged);
    f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QVERIFY(f->isInitialized());
    QCOMPARE(initializedChangedSpy.count(), 1);
    QCOMPARE(initializedChangedSpy.at(0).at(0), true);
    serviceObjectChangedSpy.clear();
    initializedChangedSpy.clear();

    m_manager->unloadAllBackends();
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    QVERIFY(!f->serviceObject());
    QVERIFY(!f->isValid());
    QVERIFY(!f->isInitialized());
    QCOMPARE(initializedChangedSpy.count(), 1);
    QCOMPARE(initializedChangedSpy.at(0).at(0), false);
}

void BaseTest::testResetServiceObject()
{
    TestBackend* backend = new TestBackend();
    m_manager->registerService(backend, backend->interfaces());

    QIfFeatureTester *f = createTester();
    QSignalSpy serviceObjectChangedSpy(f, &QIfFeatureTester::serviceObjectChanged);
    f->startAutoDiscovery();
    QVERIFY(f->serviceObject());
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
    serviceObjectChangedSpy.clear();

    QIfServiceObject* so = f->serviceObject();
    QSignalSpy soDestroyedSpy(so, &QObject::destroyed);
    serviceObjectChangedSpy.clear();

    //Reset the ServiceObject of the Feature and verify that the feature doesn't have a connection to the ServiceObject anymore
    f->setServiceObject(nullptr);

    m_manager->unloadAllBackends();
    QCOMPARE(soDestroyedSpy.count(), 1);
    QCOMPARE(serviceObjectChangedSpy.count(), 1);
}

//This construction is used to run the test twice once for a Feature and once for the FeatureListModel but show the results as separate tests.
class tst_QIfAbstractFeature : public BaseTest
{
    Q_OBJECT

public:
    tst_QIfAbstractFeature()
        : BaseTest(false)
    {}
};

class tst_QIfAbstractFeatureListModel : public BaseTest
{
    Q_OBJECT

public:
    tst_QIfAbstractFeatureListModel()
        : BaseTest(true)
    {}
};

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    tst_QIfAbstractFeature f;
    tst_QIfAbstractFeatureListModel lM;
    QTEST_SET_MAIN_SOURCE_PATH
    int featureErrors = QTest::qExec(&f, argc, argv);
    int featureModelErrors = QTest::qExec(&lM, argc, argv);

    return featureErrors + featureModelErrors;
}

#include "tst_qifabstractfeature.moc"

