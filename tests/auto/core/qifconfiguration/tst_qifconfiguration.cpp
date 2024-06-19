// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include <QtTest>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include <qifconfiguration.h>
#include <private/qifconfiguration_p.h>

#include <qifsimulationengine.h>
#include <qifabstractfeature.h>
#include <qifabstractfeaturelistmodel.h>
#include <qifservicemanager.h>

class ConfigTestFeature : public QIfAbstractFeature
{
    Q_OBJECT

public:
    ConfigTestFeature(QObject *parent = nullptr)
        : QIfAbstractFeature("testFeature", parent)
    {}

    virtual void connectToServiceObject(QIfServiceObject *serviceObject) override
    {
        QIfAbstractFeature::connectToServiceObject(serviceObject);
    }

    virtual void clearServiceObject() override
    {
    }
};

class ConfigTestFeatureListModel : public QIfAbstractFeatureListModel
{
    Q_OBJECT

public:
    ConfigTestFeatureListModel(QObject *parent = nullptr)
        : QIfAbstractFeatureListModel("testFeature", parent)
    {}

    virtual void connectToServiceObject(QIfServiceObject *serviceObject) override
    {
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
};

class ConfigTestBackend : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIfServiceInterface)

public:
    ConfigTestBackend(QObject *parent = nullptr)
        : QObject(parent)
    {}

    QString configurationId() const override
    {
        return QString("testBackend");
    }

    void updateServiceSettings(const QVariantMap &settings) override
    {
        serviceSettings = settings;
    }

    QStringList interfaces() const override
    {
        return QStringList("testFeature");
    }

    QIfFeatureInterface* interfaceInstance(const QString& interface) const override
    {
        Q_UNUSED(interface);
        return nullptr;
    }

    QVariantMap serviceSettings;
};

class tst_QIfConfiguration : public QObject
{
    Q_OBJECT


public slots:
private Q_SLOTS:
    void initTestCase();
    void cleanup();

    void invalidConfiguration();
    void noNameChange();
    void noConfiguration();

    void serviceSettings();
    void simulationFile();
    void simulationDataFile();
    void preferredBackends();
    void discoveryMode();
    void serviceObject();
    void backendUpdatesEnabled();
    void asynchronousBackendLoading();

    void emptySettingsFile();
    void emptyGroupSettingsFile();
    void singleGroupSettingsFile();
    void multipleGroupsSettingsFile();
    void invalidSettingsFile();

    void testEnvVariables();
    void testFeature();
    void testServiceObjects();

    void simulationEngine();

    void duplicateConfiguration();
    void duplicateConfigurationQML();
    void updateValuesQML();
    void testEnvVariablesQML();
private:
    template <class T> void testFeatureHelper();
};

void tst_QIfConfiguration::initTestCase()
{
}

void tst_QIfConfiguration::cleanup()
{
    qDeleteAll(QIfConfigurationManager::instance()->m_configurationHash);
    qDeleteAll(QIfConfigurationManager::instance()->m_settingsHash);

    QIfConfigurationManager::instance()->m_configurationHash.clear();
    QIfConfigurationManager::instance()->m_settingsHash.clear();

    //QIfServiceManager::instance()->unloadAllBackends();
}

void tst_QIfConfiguration::invalidConfiguration()
{
    QIfConfiguration configuration;
    QVERIFY(!configuration.isValid());

    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.setServiceSettings({{"key1", "value1"}}));
    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.setSimulationFile("foo"));
    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.setSimulationDataFile("foo"));
    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.setPreferredBackends({"backend"}));
    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.setBackendUpdatesEnabled(true));
    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.setAsynchronousBackendLoading(true));
    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.startAutoDiscovery());
    ConfigTestBackend backend;
    QIfProxyServiceObject serviceObject(&backend);
    QTest::ignoreMessage(QtWarningMsg, "Configuration Object is not usable until the name has been configured");
    QVERIFY(!configuration.setServiceObject(&serviceObject));
}

void tst_QIfConfiguration::noNameChange()
{
    QIfConfiguration configuration("config");
    QCOMPARE(configuration.name(), "config");
    QVERIFY(configuration.isValid());
    QTest::ignoreMessage(QtWarningMsg, "The name of the Configuration Object can't be changed once it has been set.");
    QVERIFY(!configuration.setName("other_config"));
}

void tst_QIfConfiguration::noConfiguration()
{
    QVERIFY(!QIfConfiguration::exists("test"));

    QVERIFY(!QIfConfiguration::areServiceSettingsSet("test"));
    QVERIFY(QIfConfiguration::serviceSettings("test").isEmpty());

    QVERIFY(!QIfConfiguration::isSimulationFileSet("test"));
    QVERIFY(QIfConfiguration::simulationFile("test").isEmpty());

    QVERIFY(!QIfConfiguration::isSimulationDataFileSet("test"));
    QVERIFY(QIfConfiguration::simulationDataFile("test").isEmpty());

    QVERIFY(!QIfConfiguration::arePreferredBackendsSet("test"));
    QVERIFY(QIfConfiguration::preferredBackends("test").isEmpty());

    QVERIFY(!QIfConfiguration::isDiscoveryModeSet("test"));
    QCOMPARE(QIfConfiguration::discoveryMode("test"), QIfAbstractFeature::InvalidAutoDiscovery);

    QVERIFY(!QIfConfiguration::isServiceObjectSet("test"));
    QCOMPARE(QIfConfiguration::serviceObject("test"), nullptr);

    QVERIFY(!QIfConfiguration::isBackendUpdatesEnabledSet("test"));
    QCOMPARE(QIfConfiguration::backendUpdatesEnabled("test"), true);

    QVERIFY(!QIfConfiguration::isAsynchronousBackendLoadingSet("test"));
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("test"), false);

    // None of the above functions should create a SettingsObject
    QVERIFY(QIfConfigurationManager::instance()->m_configurationHash.isEmpty());
    QVERIFY(QIfConfigurationManager::instance()->m_settingsHash.isEmpty());
}

void tst_QIfConfiguration::serviceSettings()
{
    QVariantMap serviceSettings = {{"key1", "value1"}, {"key2", "value2"}};
    QVERIFY(QIfConfiguration::setServiceSettings("staticGroup", serviceSettings));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::areServiceSettingsSet("staticGroup"));
    QCOMPARE(QIfConfiguration::serviceSettings("staticGroup"), serviceSettings);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.serviceSettings(), serviceSettings);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setServiceSettings(serviceSettings));
    QCOMPARE(config.serviceSettings(), serviceSettings);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::areServiceSettingsSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::serviceSettingsChanged);
    QVERIFY(spy.isValid());
    serviceSettings["key2"] = "value3";
    QVERIFY(config.setServiceSettings(serviceSettings));
    QCOMPARE(config.serviceSettings(), serviceSettings);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], serviceSettings);
}

void tst_QIfConfiguration::simulationFile()
{
    // call static setter
    QString simulationFile = "simulationFile";
    QVERIFY(QIfConfiguration::setSimulationFile("staticGroup", simulationFile));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::isSimulationFileSet("staticGroup"));
    QCOMPARE(QIfConfiguration::simulationFile("staticGroup"), simulationFile);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.simulationFile(), simulationFile);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setSimulationFile(simulationFile));
    QCOMPARE(config.simulationFile(), simulationFile);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::isSimulationFileSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::simulationFileChanged);
    QVERIFY(spy.isValid());
    simulationFile = "otherFile";
    QVERIFY(config.setSimulationFile(simulationFile));
    QCOMPARE(config.simulationFile(), simulationFile);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], simulationFile);
}

void tst_QIfConfiguration::simulationDataFile()
{
    // call static setter
    QString simulationDataFile = "simulationDataFile";
    QVERIFY(QIfConfiguration::setSimulationDataFile("staticGroup", simulationDataFile));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::isSimulationDataFileSet("staticGroup"));
    QCOMPARE(QIfConfiguration::simulationDataFile("staticGroup"), simulationDataFile);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.simulationDataFile(), simulationDataFile);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setSimulationDataFile(simulationDataFile));
    QCOMPARE(config.simulationDataFile(), simulationDataFile);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::isSimulationDataFileSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::simulationDataFileChanged);
    QVERIFY(spy.isValid());
    simulationDataFile = "otherFile";
    QVERIFY(config.setSimulationDataFile(simulationDataFile));
    QCOMPARE(config.simulationDataFile(), simulationDataFile);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], simulationDataFile);
}

void tst_QIfConfiguration::preferredBackends()
{
    // call static setter
    QStringList preferredBackends = QStringList("*foo");
    QVERIFY(QIfConfiguration::setPreferredBackends("staticGroup", preferredBackends));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::arePreferredBackendsSet("staticGroup"));
    QCOMPARE(QIfConfiguration::preferredBackends("staticGroup"), preferredBackends);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.preferredBackends(), preferredBackends);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setPreferredBackends(preferredBackends));
    QCOMPARE(config.preferredBackends(), preferredBackends);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::arePreferredBackendsSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::preferredBackendsChanged);
    QVERIFY(spy.isValid());
    preferredBackends = QStringList("*bar");
    QVERIFY(config.setPreferredBackends(preferredBackends));
    QCOMPARE(config.preferredBackends(), preferredBackends);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], preferredBackends);
}

void tst_QIfConfiguration::discoveryMode()
{
    // call static setter
    QIfAbstractFeature::DiscoveryMode discoveryMode = QIfAbstractFeature::LoadOnlyProductionBackends;
    QVERIFY(QIfConfiguration::setDiscoveryMode("staticGroup", discoveryMode));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::isDiscoveryModeSet("staticGroup"));
    QCOMPARE(QIfConfiguration::discoveryMode("staticGroup"), discoveryMode);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.discoveryMode(), discoveryMode);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setDiscoveryMode(discoveryMode));
    QCOMPARE(config.discoveryMode(), discoveryMode);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::isDiscoveryModeSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::discoveryModeChanged);
    QVERIFY(spy.isValid());
    discoveryMode = QIfAbstractFeature::LoadOnlySimulationBackends;
    QVERIFY(config.setDiscoveryMode(discoveryMode));
    QCOMPARE(config.discoveryMode(), discoveryMode);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], QVariant::fromValue(discoveryMode));
}

void tst_QIfConfiguration::serviceObject()
{
    // call static setter
    ConfigTestBackend backend;
    QIfProxyServiceObject serviceObject(&backend);
    QVERIFY(QIfConfiguration::setServiceObject("staticGroup", &serviceObject));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::isServiceObjectSet("staticGroup"));
    QCOMPARE(QIfConfiguration::serviceObject("staticGroup"), &serviceObject);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.serviceObject(), &serviceObject);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setServiceObject(&serviceObject));
    QCOMPARE(config.serviceObject(), &serviceObject);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::isServiceObjectSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::serviceObjectChanged);
    QVERIFY(spy.isValid());
    ConfigTestBackend backend2;
    QIfProxyServiceObject serviceObject2(&backend2);
    QVERIFY(config.setServiceObject(&serviceObject2));
    QCOMPARE(config.serviceObject(), &serviceObject2);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], QVariant::fromValue<QIfServiceObject*>(&serviceObject2));
}

void tst_QIfConfiguration::backendUpdatesEnabled()
{
    // call static setter
    QVERIFY(QIfConfiguration::setBackendUpdatesEnabled("staticGroup", true));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::isBackendUpdatesEnabledSet("staticGroup"));
    QCOMPARE(QIfConfiguration::backendUpdatesEnabled("staticGroup"), true);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.backendUpdatesEnabled(), true);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setBackendUpdatesEnabled(false));
    QCOMPARE(config.backendUpdatesEnabled(), false);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::isBackendUpdatesEnabledSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::backendUpdatesEnabledChanged);
    QVERIFY(spy.isValid());
    QVERIFY(config.setBackendUpdatesEnabled(true));
    QCOMPARE(config.backendUpdatesEnabled(), true);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], true);
}

void tst_QIfConfiguration::asynchronousBackendLoading()
{
    // call static setter
    QVERIFY(QIfConfiguration::setAsynchronousBackendLoading("staticGroup", true));
    QVERIFY(QIfConfiguration::exists("staticGroup"));
    QVERIFY(QIfConfiguration::isAsynchronousBackendLoadingSet("staticGroup"));
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("staticGroup"), true);

    // Verify that reading the setting using the object API works as well
    QIfConfiguration staticGroupConfig("staticGroup");
    QCOMPARE(staticGroupConfig.asynchronousBackendLoading(), true);

    // Create Configuration and call that method
    QIfConfiguration config("objectGroup");
    QVERIFY(config.isValid());
    QVERIFY(config.setAsynchronousBackendLoading(true));
    QCOMPARE(config.asynchronousBackendLoading(), true);
    QVERIFY(QIfConfiguration::exists("objectGroup"));
    QVERIFY(QIfConfiguration::isAsynchronousBackendLoadingSet("objectGroup"));

    // Test the change signal
    QSignalSpy spy(&config, &QIfConfiguration::asynchronousBackendLoadingChanged);
    QVERIFY(spy.isValid());
    QVERIFY(config.setAsynchronousBackendLoading(false));
    QCOMPARE(config.asynchronousBackendLoading(), false);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.data()[0][0], false);
}

void tst_QIfConfiguration::emptySettingsFile()
{
    // Reading a empty file shouldn't cause any problem
    QIfConfigurationManager::instance()->readInitialSettings(QFINDTESTDATA("testdata/ini/empty_settings.ini"));
    QVERIFY(QIfConfigurationManager::instance()->m_configurationHash.isEmpty());
    QVERIFY(QIfConfigurationManager::instance()->m_settingsHash.isEmpty());
}

void tst_QIfConfiguration::emptyGroupSettingsFile()
{
    // Reading a file with an empty groupd shouldn't cause any problem
    QIfConfigurationManager::instance()->readInitialSettings(QFINDTESTDATA("testdata/ini/empty_group.ini"));
    QVERIFY(QIfConfigurationManager::instance()->m_configurationHash.isEmpty());
    QVERIFY(QIfConfigurationManager::instance()->m_settingsHash.isEmpty());
}

void tst_QIfConfiguration::singleGroupSettingsFile()
{
    QIfConfigurationManager::instance()->readInitialSettings(QFINDTESTDATA("testdata/ini/single_group.ini"));
    QVERIFY(QIfConfiguration::exists("group1"));
    QVERIFY(QIfConfiguration::isSimulationFileSet("group1"));
    QCOMPARE(QIfConfiguration::simulationFile("group1"), "simulationFile");
    QVERIFY(QIfConfiguration::isSimulationDataFileSet("group1"));
    QCOMPARE(QIfConfiguration::simulationDataFile("group1"), "simulationDataFile");
    QVERIFY(!QIfConfiguration::exists("group2"));
}

void tst_QIfConfiguration::multipleGroupsSettingsFile()
{
    QIfConfigurationManager::instance()->readInitialSettings(QFINDTESTDATA("testdata/ini/multiple_groups.ini"));
    QVERIFY(QIfConfiguration::exists("group1"));
    QVERIFY(QIfConfiguration::arePreferredBackendsSet("group1"));
    QCOMPARE(QIfConfiguration::preferredBackends("group1"), QStringList({"backend1", "backend2"}));
    QVERIFY(QIfConfiguration::areServiceSettingsSet("group1"));
    QCOMPARE(QIfConfiguration::serviceSettings("group1"), QVariantMap({{"key1", "value1"}, {"key2", "value2"}}));
    QVERIFY(QIfConfiguration::exists("group2"));
    QVERIFY(QIfConfiguration::isDiscoveryModeSet("group2"));
    QCOMPARE(QIfConfiguration::discoveryMode("group2"), QIfAbstractFeature::LoadOnlySimulationBackends);
    QVERIFY(QIfConfiguration::isAsynchronousBackendLoadingSet("group2"));
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("group2"), true);
}

void tst_QIfConfiguration::invalidSettingsFile()
{
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Ignoring malformed discoveryMode: 'InvalidValue'. Possible values are:(.*)"));
    QIfConfigurationManager::instance()->readInitialSettings(QFINDTESTDATA("testdata/ini/invalid.ini"));
    QVERIFY(!QIfConfiguration::exists("group1"));
    QVERIFY(!QIfConfiguration::isDiscoveryModeSet("group1"));
}

void tst_QIfConfiguration::testEnvVariables()
{
    QString simulation1 = QFINDTESTDATA("testdata/simulation/default_simulation.qml");
    QString simulation2 = QFINDTESTDATA("testdata/simulation/override_simulation.qml");
    QString simulationData1 = QFINDTESTDATA("testdata/simulation/default_simulation_data.json");
    QString simulationData2 = QFINDTESTDATA("testdata/simulation/override_simulation_data.json");

    qputenv("QTIF_SIMULATION_OVERRIDE", "config1=" + simulation1.toLocal8Bit() + ";config2=" + simulation2.toLocal8Bit());
    qputenv("QTIF_SIMULATION_DATA_OVERRIDE", "config1=" + simulationData1.toLocal8Bit() + ";config2=" + simulationData2.toLocal8Bit());
    qputenv("QTIF_DISCOVERY_MODE_OVERRIDE", "config1=LoadOnlyProductionBackends;config2=AutoDiscovery");
    qputenv("QTIF_PREFERRED_BACKENDS_OVERRIDE", "config1=qtro*,*;config2=*simulation*");
    qputenv("QTIF_ASYNCHRONOUS_BACKEND_LOADING_OVERRIDE", "config1=true;config2=0");

    // parse the settingsFile and the env variables
    QIfConfigurationManager::instance()->readInitialSettings(QFINDTESTDATA("testdata/ini/empty_settings.ini"));

    qunsetenv("QTIF_SIMULATION_OVERRIDE");
    qunsetenv("QTIF_SIMULATION_DATA_OVERRIDE");
    qunsetenv("QTIF_DISCOVERY_MODE_OVERRIDE");
    qunsetenv("QTIF_PREFERRED_BACKENDS_OVERRIDE");
    qunsetenv("QTIF_ASYNCHRONOUS_BACKEND_LOADING_OVERRIDE");

    QVERIFY(QIfConfiguration::exists("config1"));
    QVERIFY(QIfConfiguration::exists("config2"));

    QVERIFY(QIfConfiguration::isSimulationFileSet("config1"));
    QVERIFY(QIfConfiguration::isSimulationFileSet("config2"));
    QCOMPARE(QIfConfiguration::simulationFile("config1"), simulation1);
    QCOMPARE(QIfConfiguration::simulationFile("config2"), simulation2);
    // Try to set a new value (which can't be done because of the override)
    QTest::ignoreMessage(QtWarningMsg, "Changing the simulationFile is not possible, because the QTIF_SIMULATION_OVERRIDE env variable has been set.");
    QVERIFY(!QIfConfiguration::setSimulationFile("config1", "newValue"));
    QCOMPARE(QIfConfiguration::simulationFile("config1"), simulation1);

    QVERIFY(QIfConfiguration::isSimulationDataFileSet("config1"));
    QVERIFY(QIfConfiguration::isSimulationDataFileSet("config2"));
    QCOMPARE(QIfConfiguration::simulationDataFile("config1"), simulationData1);
    QCOMPARE(QIfConfiguration::simulationDataFile("config2"), simulationData2);
    // Try to set a new value (which can't be done because of the override)
    QTest::ignoreMessage(QtWarningMsg, "Changing the simulationDataFile is not possible, because the QTIF_SIMULATION_DATA_OVERRIDE env variable has been set.");
    QVERIFY(!QIfConfiguration::setSimulationDataFile("config1", "newValue"));
    QCOMPARE(QIfConfiguration::simulationDataFile("config1"), simulationData1);

    QVERIFY(QIfConfiguration::isDiscoveryModeSet("config1"));
    QVERIFY(QIfConfiguration::isDiscoveryModeSet("config2"));
    QCOMPARE(QIfConfiguration::discoveryMode("config1"), QIfAbstractFeature::LoadOnlyProductionBackends);
    QCOMPARE(QIfConfiguration::discoveryMode("config2"), QIfAbstractFeature::AutoDiscovery);
    // Try to set a new value (which can't be done because of the override)
    QTest::ignoreMessage(QtWarningMsg, "Changing the discoveryMode is not possible, because the QTIF_DISCOVERY_MODE_OVERRIDE env variable has been set.");
    QVERIFY(!QIfConfiguration::setDiscoveryMode("config1", QIfAbstractFeature::NoAutoDiscovery));
    QCOMPARE(QIfConfiguration::discoveryMode("config1"), QIfAbstractFeature::LoadOnlyProductionBackends);

    QVERIFY(QIfConfiguration::arePreferredBackendsSet("config1"));
    QVERIFY(QIfConfiguration::arePreferredBackendsSet("config2"));
    QCOMPARE(QIfConfiguration::preferredBackends("config1"), QStringList({"qtro*", "*"}));
    QCOMPARE(QIfConfiguration::preferredBackends("config2"), QStringList({"*simulation*"}));
    // Try to set a new value (which can't be done because of the override)
    QTest::ignoreMessage(QtWarningMsg, "Changing the preferredBackends is not possible, because the QTIF_PREFERRED_BACKENDS_OVERRIDE env variable has been set.");
    QVERIFY(!QIfConfiguration::setPreferredBackends("config1", QStringList("newValue")));
    QCOMPARE(QIfConfiguration::preferredBackends("config1"), QStringList({"qtro*", "*"}));

    QVERIFY(QIfConfiguration::isAsynchronousBackendLoadingSet("config1"));
    QVERIFY(QIfConfiguration::isAsynchronousBackendLoadingSet("config2"));
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("config1"), true);
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("config2"), false);
    // Try to set a new value (which can't be done because of the override)
    QTest::ignoreMessage(QtWarningMsg, "Changing asynchronousBackendLoading is not possible, because the QTIF_ASYNCHRONOUS_BACKEND_LOADING_OVERRIDE env variable has been set.");
    QVERIFY(!QIfConfiguration::setAsynchronousBackendLoading("config1", false));
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("config1"), true);
}

template <class T> void tst_QIfConfiguration::testFeatureHelper()
{
    // Create a Feature and verify the default values
    auto testFeature = new T;
    testFeature->setConfigurationId("config1");
    QCOMPARE(testFeature->discoveryMode(), QIfAbstractFeature::AutoDiscovery);
    QCOMPARE(testFeature->serviceObject(), nullptr);
    QCOMPARE(testFeature->preferredBackends(), QStringList());
    QCOMPARE(testFeature->backendUpdatesEnabled(), true);
    QCOMPARE(testFeature->asynchronousBackendLoading(), false);

    // Set a configuration and make sure the matching Feature changes as well
    QSignalSpy discoverySpy(testFeature, &T::discoveryModeChanged);
    QVERIFY(QIfConfiguration::setDiscoveryMode("config1", QIfAbstractFeature::LoadOnlyProductionBackends));
    QCOMPARE(testFeature->discoveryMode(), QIfAbstractFeature::LoadOnlyProductionBackends);
    QCOMPARE(discoverySpy.count(), 1);
    QSignalSpy backendUpdatesEnabledSpy(testFeature, &T::backendUpdatesEnabledChanged);
    QVERIFY(QIfConfiguration::setBackendUpdatesEnabled("config1", false));
    QCOMPARE(testFeature->backendUpdatesEnabled(), false);
    QCOMPARE(backendUpdatesEnabledSpy.count(), 1);
    QSignalSpy asynchronousBackendLoadingdSpy(testFeature, &T::asynchronousBackendLoadingChanged);
    QVERIFY(QIfConfiguration::setAsynchronousBackendLoading("config1", true));
    QCOMPARE(testFeature->asynchronousBackendLoading(), true);
    QCOMPARE(asynchronousBackendLoadingdSpy.count(), 1);

    auto backend = new ConfigTestBackend;
    QIfServiceManager::instance()->registerService(backend, QStringList({"testFeature"}));
    QIfProxyServiceObject serviceObject(backend);
    QSignalSpy serviceObjectSpy(testFeature, &T::serviceObjectChanged);
    QVERIFY(QIfConfiguration::setServiceObject("config1", &serviceObject));
    QCOMPARE(testFeature->serviceObject(), &serviceObject);
    QCOMPARE(serviceObjectSpy.count(), 1);

    QSignalSpy preferredBackendSpy(testFeature, &T::preferredBackendsChanged);
    QVERIFY(QIfConfiguration::setPreferredBackends("config1", QStringList({"*simulation*"})));
    QCOMPARE(testFeature->preferredBackends(), QStringList({"*simulation*"}));
    QCOMPARE(preferredBackendSpy.count(), 1);

    // Create a new feature with a configId that already has a configuration
    auto testFeature2 = new T;
    testFeature2->setConfigurationId("config1");
    QCOMPARE(testFeature2->discoveryMode(), QIfAbstractFeature::LoadOnlyProductionBackends);
    QCOMPARE(testFeature2->serviceObject(), &serviceObject);
    QCOMPARE(testFeature2->preferredBackends(), QStringList({"*simulation*"}));
    QCOMPARE(testFeature2->backendUpdatesEnabled(), false);
    QCOMPARE(testFeature2->asynchronousBackendLoading(), true);

    // Reset the serviceObject and settings and test the auto discovery
    QVERIFY(QIfConfiguration::setServiceObject("config1", nullptr));
    QVERIFY(QIfConfiguration::setPreferredBackends("config1", QStringList()));
    QVERIFY(QIfConfiguration::setAsynchronousBackendLoading("config1", false));
    QCOMPARE(testFeature->serviceObject(), nullptr);
    QCOMPARE(testFeature2->serviceObject(), nullptr);
    QIfConfiguration::startAutoDiscovery("config1");
    QVERIFY(testFeature->serviceObject());
    QVERIFY(testFeature2->serviceObject());
    QIfServiceManager::instance()->unloadAllBackends();


    // Make sure the pointers to the registered features are removed when the features are deleted
    delete testFeature2;
    delete testFeature;
    auto settingsObject = QIfConfigurationManager::instance()->m_settingsHash.value("config1");
    QVERIFY(settingsObject);
    QVERIFY(settingsObject->features.isEmpty());
}

void tst_QIfConfiguration::testFeature()
{
    testFeatureHelper<ConfigTestFeature>();
    cleanup();
    testFeatureHelper<ConfigTestFeatureListModel>();
}

void tst_QIfConfiguration::testServiceObjects()
{
    auto backend = new ConfigTestBackend;
    QIfServiceManager::instance()->registerService(backend, QStringList({"testFeature"}));

    std::unique_ptr<ConfigTestFeature> testFeature(new ConfigTestFeature);
    testFeature->startAutoDiscovery();
    QVERIFY(testFeature->serviceObject());

    QIfServiceObject *so = testFeature->serviceObject();
    QCOMPARE(so->configurationId(), "testBackend");
    QCOMPARE(so->serviceSettings(), QVariantMap());

    QVERIFY(QIfConfiguration::setServiceSettings("testBackend", QVariantMap({{"key1", "value1"}})));
    QCOMPARE(so->serviceSettings(), QVariantMap({{"key1", "value1"}}));

    QIfServiceManager::instance()->unloadAllBackends();

    auto backend2 = new ConfigTestBackend;
    QIfServiceManager::instance()->registerService(backend2, QStringList({"testFeature"}));
    auto testFeature2 = new ConfigTestFeature;
    testFeature2->startAutoDiscovery();
    QVERIFY(testFeature2->serviceObject());

    QCOMPARE(backend2->serviceSettings, QVariantMap({{"key1", "value1"}}));
    delete testFeature2;
}

void tst_QIfConfiguration::simulationEngine()
{
    QIfSimulationEngine *engine = new QIfSimulationEngine("testEngine");
    engine->loadSimulationData(QFINDTESTDATA("testdata/simulation/default_simulation_data.json"));
    engine->loadSimulation(QUrl::fromLocalFile(QFINDTESTDATA("testdata/simulation/default_simulation.qml")));
    QVERIFY(!engine->rootObjects().isEmpty());
    QCOMPARE(engine->rootObjects().at(0)->objectName(), "defaultSimulation");
    delete engine;

    QVERIFY(QIfConfiguration::setSimulationDataFile("testEngine", QFINDTESTDATA("testdata/simulation/override_simulation_data.json")));
    QVERIFY(QIfConfiguration::setSimulationFile("testEngine", QFINDTESTDATA("testdata/simulation/override_simulation.qml")));
    engine = new QIfSimulationEngine("testEngine");
    // Verify that values from the configuration are preferred over the hardcoded values
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Using simulation data override from QIfConfiguration.*"));
    engine->loadSimulationData(QFINDTESTDATA("testdata/simulation/default_simulation_data.json"));
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("Using simulation override from QIfConfiguration.*"));
    engine->loadSimulation(QUrl::fromLocalFile(QFINDTESTDATA("testdata/simulation/default_simulation.qml")));
    QVERIFY(!engine->rootObjects().isEmpty());
    QCOMPARE(engine->rootObjects().at(0)->objectName(), "overrideSimulation");
    delete engine;
}

void tst_QIfConfiguration::duplicateConfiguration()
{
    QIfConfiguration config("config1");
    QVERIFY(config.isValid());

    QTest::ignoreMessage(QtWarningMsg, QRegularExpression("A Configuration Object with this name already exists."));
    QIfConfiguration duplicate_config("config1");
    QVERIFY(!duplicate_config.isValid());
}

void tst_QIfConfiguration::duplicateConfigurationQML()
{
    QQmlApplicationEngine engine;
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".* QML InterfaceFrameworkConfiguration: A Configuration Object with this name already exists."));
    engine.load(QFINDTESTDATA("testdata/qml/duplicate_configuration.qml"));
    QVERIFY(!engine.rootObjects().isEmpty());
}

void tst_QIfConfiguration::updateValuesQML()
{
    ConfigTestBackend backend1;
    QIfProxyServiceObject serviceObject1(&backend1);
    ConfigTestBackend backend2;
    QIfProxyServiceObject serviceObject2(&backend2);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("serviceObject1", &serviceObject1);
    engine.rootContext()->setContextProperty("serviceObject2", &serviceObject2);
    engine.load(QFINDTESTDATA("testdata/qml/update_values.qml"));
    QVERIFY(!engine.rootObjects().isEmpty());
    QObject *rootObject = engine.rootObjects().at(0);

    QCOMPARE(QIfConfiguration::simulationFile("config1"), "simulationFile");
    QCOMPARE(QIfConfiguration::simulationDataFile("config1"), "simulationDataFile");
    QCOMPARE(QIfConfiguration::serviceSettings("config1"), QVariantMap({{"key", "value"}}));
    QCOMPARE(QIfConfiguration::preferredBackends("config1"), QStringList({"*backend"}));
    QCOMPARE(QIfConfiguration::discoveryMode("config1"), QIfAbstractFeature::LoadOnlyProductionBackends);
    QCOMPARE(QIfConfiguration::serviceObject("config1"), &serviceObject1);
    QCOMPARE(QIfConfiguration::backendUpdatesEnabled("config1"), false);
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("config1"), true);

    rootObject->setProperty("triggerChange", false);

    QCOMPARE(QIfConfiguration::simulationFile("config1"), "otherFile");
    QCOMPARE(QIfConfiguration::simulationDataFile("config1"), "otherFile");
    QCOMPARE(QIfConfiguration::serviceSettings("config1"), QVariantMap({{"key2", "value2"}}));
    QCOMPARE(QIfConfiguration::preferredBackends("config1"), QStringList({"value1", "value2"}));
    QCOMPARE(QIfConfiguration::discoveryMode("config1"), QIfAbstractFeature::LoadOnlySimulationBackends);
    QCOMPARE(QIfConfiguration::serviceObject("config1"), &serviceObject2);
    QCOMPARE(QIfConfiguration::backendUpdatesEnabled("config1"), true);
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("config1"), false);
}

void tst_QIfConfiguration::testEnvVariablesQML()
{
    QString simulation1 = QFINDTESTDATA("testdata/simulation/default_simulation.qml");
    QString simulationData1 = QFINDTESTDATA("testdata/simulation/default_simulation_data.json");

    qputenv("QTIF_SIMULATION_OVERRIDE", "config1=" + simulation1.toLocal8Bit());
    qputenv("QTIF_SIMULATION_DATA_OVERRIDE", "config1=" + simulationData1.toLocal8Bit());
    qputenv("QTIF_DISCOVERY_MODE_OVERRIDE", "config1=AutoDiscovery");
    qputenv("QTIF_PREFERRED_BACKENDS_OVERRIDE", "config1=qtro*,*");
    qputenv("QTIF_ASYNCHRONOUS_BACKEND_LOADING_OVERRIDE", "config1=true");

    // parse the settingsFile and the env variables
    QIfConfigurationManager::instance()->readInitialSettings(QFINDTESTDATA("testdata/ini/empty_settings.ini"));

    qunsetenv("QTIF_SIMULATION_OVERRIDE");
    qunsetenv("QTIF_SIMULATION_DATA_OVERRIDE");
    qunsetenv("QTIF_DISCOVERY_MODE_OVERRIDE");
    qunsetenv("QTIF_PREFERRED_BACKENDS_OVERRIDE");
    qunsetenv("QTIF_ASYNCHRONOUS_BACKEND_LOADING_OVERRIDE");

    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*Changing the simulationFile is not possible, because the QTIF_SIMULATION_OVERRIDE env variable has been set."));
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*Changing the simulationDataFile is not possible, because the QTIF_SIMULATION_DATA_OVERRIDE env variable has been set."));
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*Changing the discoveryMode is not possible, because the QTIF_DISCOVERY_MODE_OVERRIDE env variable has been set."));
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*Changing the preferredBackends is not possible, because the QTIF_PREFERRED_BACKENDS_OVERRIDE env variable has been set."));
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*Changing asynchronousBackendLoading is not possible, because the QTIF_ASYNCHRONOUS_BACKEND_LOADING_OVERRIDE env variable has been set."));
    QQmlApplicationEngine engine;
    engine.load(QFINDTESTDATA("testdata/qml/env_values.qml"));
    QVERIFY(!engine.rootObjects().isEmpty());
    QObject *rootObject = engine.rootObjects().at(0);

    QCOMPARE(QIfConfiguration::simulationFile("config1"), simulation1);
    QCOMPARE(QIfConfiguration::simulationDataFile("config1"), simulationData1);
    QCOMPARE(QIfConfiguration::preferredBackends("config1"), QStringList({"qtro*", "*"}));
    QCOMPARE(QIfConfiguration::discoveryMode("config1"), QIfAbstractFeature::AutoDiscovery);
    QCOMPARE(QIfConfiguration::asynchronousBackendLoading("config1"), true);

    QTest::failOnWarning(QRegularExpression(".*Changing the simulationFile is not possible, because the QTIF_SIMULATION_OVERRIDE env variable has been set."));
    QTest::failOnWarning(QRegularExpression(".*Changing the simulationDataFile is not possible, because the QTIF_SIMULATION_DATA_OVERRIDE env variable has been set."));
    QTest::failOnWarning(QRegularExpression(".*Changing the discoveryMode is not possible, because the QTIF_DISCOVERY_MODE_OVERRIDE env variable has been set."));
    QTest::failOnWarning(QRegularExpression(".*Changing the preferredBackends is not possible, because the QTIF_PREFERRED_BACKENDS_OVERRIDE env variable has been set."));
    QTest::failOnWarning(QRegularExpression(".*Changing asynchronousBackendLoading is not possible, because the QTIF_ASYNCHRONOUS_BACKEND_LOADING_OVERRIDE env variable has been set."));
    rootObject->setProperty("ignoreOverrideWarnings", true);
    rootObject->setProperty("triggerChange", false);
}

QTEST_MAIN(tst_QIfConfiguration)

#include "tst_qifconfiguration.moc"
