// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifconfiguration.h"
#include "qifconfiguration_p.h"
#include "qifabstractfeature.h"

#include "qifqmlconversion_helper.h"

#include <QLibraryInfo>
#include <QDir>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(qLcIfConfig, "qt.if.configuration");

#define Q_CHECK_SETTINGSOBJECT(return_value) \
if (!d->m_settingsObject) { \
    qtif_qmlOrCppWarning(this, "Configuration Object is not usable until the name has been configured"); \
    return return_value; \
}

QIfConfigurationManager::QIfConfigurationManager()
{
    const QString configFileName = QStringLiteral("qtifconfig.ini");

    const QString configPath
                = QDir(QLibraryInfo::path(QLibraryInfo::DataPath)).absoluteFilePath(configFileName);
    readInitialSettings(configPath);
}

QIfConfigurationManager *QIfConfigurationManager::instance()
{
    static QIfConfigurationManager s_manager;
    return &s_manager;
}


QIfAbstractFeature::DiscoveryMode discoveryModeFromString(const QString &modeString)
{
    QMetaEnum me = QMetaEnum::fromType<QIfAbstractFeature::DiscoveryMode>();
    bool ok = false;
    int value = me.keyToValue(modeString.toUtf8().constData(), &ok);
    if (ok) {
        return static_cast<QIfAbstractFeature::DiscoveryMode>(value);
    } else {
        QByteArray values;
        for (int i=0; i<me.keyCount();i++) {
            if (i == 0)
                values += me.key(i);
            else
                values += QByteArray(", ") + me.key(i);
        }
        qCWarning(qLcIfConfig, "Ignoring malformed discoveryMode: '%s'. Possible values are: '%s'", modeString.toUtf8().constData(), values.constData());
    }

    return QIfAbstractFeature::InvalidAutoDiscovery;
}

QVariantMap QIfConfigurationManager::readGroup(QSettings *settings, QAnyStringView group)
{
    QVariantMap map;
    settings->beginGroup(group);
    for (const QString& key : settings->childKeys())
        map.insert(key, settings->value(key));
    for (const QString& group : settings->childGroups())
        map.insert(group, readGroup(settings, group));
    settings->endGroup();
    return map;
}

void QIfConfigurationManager::readInitialSettings(const QString &configPath)
{
    qCDebug(qLcIfConfig) << "Loading initial settings from " << configPath;

    QSettings settings(configPath, QSettings::IniFormat);

    for (const QString& group : settings.childGroups()) {
        auto settingsObject = new QIfSettingsObject;

        settings.beginGroup(group);
        settingsObject->simulationFileSet = settings.contains("simulationFile");
        settingsObject->simulationFile = settings.value("simulationFile").toString();
        settingsObject->simulationDataFileSet = settings.contains("simulationDataFile");
        settingsObject->simulationDataFile = settings.value("simulationDataFile").toString();
        settingsObject->preferredBackendsSet = settings.contains("preferredBackends");
        settingsObject->preferredBackends = settings.value("preferredBackends").toStringList();
        QVariant discoveryModeVariant = settings.value("discoveryMode");

        if (settings.childGroups().contains("serviceSettings")) {
            settingsObject->serviceSettingsSet = true;
            settingsObject->serviceSettings = readGroup(&settings, "serviceSettings");
        }
        settings.endGroup();

        if (discoveryModeVariant.isValid()) {
            auto discoveryMode = discoveryModeFromString(discoveryModeVariant.toString());
            if (discoveryMode == QIfAbstractFeature::InvalidAutoDiscovery)
                return;
            settingsObject->discoveryMode = discoveryMode;
            settingsObject->discoveryModeSet = true;
        }

        m_settingsHash.insert(group, settingsObject);
    }

    parseEnv(qgetenv("QTIF_SIMULATION_OVERRIDE"), [this](const QString &group, const QString& value) {
        if (!QFile::exists(value)) {
            qCWarning(qLcIfConfig, "Ignoring malformed override: File does not exist: '%s'", value.toUtf8().constData());
            return;
        }

        auto *so = settingsObject(group, true);
        so->simulationFile = value;
        so->simulationFileSet = true;
        so->simulationFileEnvOverride = true;
    });

    parseEnv(qgetenv("QTIF_SIMULATION_DATA_OVERRIDE"), [this](const QString &group, const QString& value) {
        if (!QFile::exists(value)) {
            qCWarning(qLcIfConfig, "Ignoring malformed override: File does not exist: '%s'", value.toUtf8().constData());
            return;
        }

        auto *so = settingsObject(group, true);
        so->simulationDataFile = value;
        so->simulationDataFileSet = true;
        so->simulationDataFileEnvOverride = true;
    });

    parseEnv(qgetenv("QTIF_DISCOVERY_MODE_OVERRIDE"), [this](const QString &group, const QString& value) {
        auto *so = settingsObject(group, true);
        auto discoveryMode = discoveryModeFromString(value);
        if (discoveryMode == QIfAbstractFeature::InvalidAutoDiscovery)
            return;

        so->discoveryMode = discoveryMode;
        so->discoveryModeSet = true;
        so->discoveryModeEnvOverride = true;
    });

    parseEnv(qgetenv("QTIF_PREFERRED_BACKENDS_OVERRIDE"), [this](const QString &group, const QString& value) {
        auto *so = settingsObject(group, true);
        auto preferredBackends = value.split(u',', Qt::SkipEmptyParts);
        if (preferredBackends.isEmpty()) {
            qCWarning(qLcIfConfig, "Ignoring malformed override: List is empty or couldn't be parsed: '%s'", value.toUtf8().constData());
            return;
        }

        so->preferredBackends = preferredBackends;
        so->preferredBackendsSet = true;
        so->preferredBackendsEnvOverride = true;
    });
}

QIfSettingsObject *QIfConfigurationManager::settingsObject(const QString &group, bool create)
{
    QIfSettingsObject *settingsObject = nullptr;
    if (m_settingsHash.contains(group)) {
        settingsObject = m_settingsHash.value(group);
    } else if (create) {
        settingsObject = new QIfSettingsObject;
        m_settingsHash.insert(group, settingsObject);
    }
    return settingsObject;
}

void QIfConfigurationManager::addServiceObject(const QString &group, QIfProxyServiceObject *serviceObject)
{
    Q_ASSERT(serviceObject);
    QIfSettingsObject *so = settingsObject(group, true);
    Q_ASSERT(so);

    so->serviceObjects.append(serviceObject);
    if (so->serviceSettingsSet) {
        qCDebug(qLcIfConfig) << "Updating Service Settings of" << serviceObject << "with" << so->serviceSettings;
        serviceObject->updateServiceSettings(so->serviceSettings);
    }
}

void QIfConfigurationManager::removeServiceObject(const QString &group, QIfProxyServiceObject *serviceObject)
{
    Q_ASSERT(serviceObject);
    QIfSettingsObject *so = settingsObject(group);
    if (!so)
        return;

    so->serviceObjects.removeAll(serviceObject);
}

void QIfConfigurationManager::addAbstractFeature(const QString &group, QIfAbstractFeature *feature)
{
    Q_ASSERT(feature);
    QIfSettingsObject *so = settingsObject(group, true);
    Q_ASSERT(so);

    so->features.append(feature);
    if (so->discoveryModeSet) {
        qCDebug(qLcIfConfig) << "Updating discoveryMode of" << feature << "with" << so->discoveryMode;
        feature->setDiscoveryMode(so->discoveryMode);
    }

    if (so->preferredBackendsSet) {
        qCDebug(qLcIfConfig) << "Updating preferredBackends of" << feature << "with" << so->preferredBackends;
        feature->setPreferredBackends(so->preferredBackends);
    }

    if (so->serviceObjectSet) {
        qCDebug(qLcIfConfig) << "Updating serviceObject of" << feature << "with" << so->serviceObject;
        feature->setServiceObject(so->serviceObject);
    }
}

void QIfConfigurationManager::removeAbstractFeature(const QString &group, QIfAbstractFeature *feature)
{
    Q_ASSERT(feature);
    QIfSettingsObject *so = settingsObject(group);
    Q_ASSERT(so);

    so->features.removeAll(feature);
}

bool QIfConfigurationManager::setServiceSettings(QIfSettingsObject *so, const QVariantMap &serviceSettings)
{
    Q_ASSERT(so);
    so->serviceSettings = serviceSettings;
    so->serviceSettingsSet = true;

    for (auto &serviceObject : std::as_const(so->serviceObjects)) {
        if (!serviceObject)
            continue;
        qCDebug(qLcIfConfig) << "Updating Service Settings of" << serviceObject << "with" << serviceSettings;
        serviceObject->updateServiceSettings(serviceSettings);
    }
    return true;
}

bool QIfConfigurationManager::setSimulationFile(QIfConfiguration *config, QIfSettingsObject *so, const QString &simulationFile)
{
    Q_ASSERT(so);
    if (so->simulationFileEnvOverride) {
        if (!config || !config->ignoreOverrideWarnings())
            qtif_qmlOrCppWarning(config, "Changing the simulationFile is not possible, because the QTIF_SIMULATION_OVERRIDE env variable has been set.");
        return false;
    }
    so->simulationFile = simulationFile;
    so->simulationFileSet = true;
    return true;
}

bool QIfConfigurationManager::setSimulationDataFile(QIfConfiguration *config, QIfSettingsObject *so, const QString &simulationDataFile)
{
    Q_ASSERT(so);
    if (so->simulationDataFileEnvOverride) {
        if (!config || !config->ignoreOverrideWarnings())
            qtif_qmlOrCppWarning(config, "Changing the simulationDataFile is not possible, because the QTIF_SIMULATION_DATA_OVERRIDE env variable has been set.");
        return false;
    }
    so->simulationDataFile = simulationDataFile;
    so->simulationDataFileSet = true;
    return true;
}

bool QIfConfigurationManager::setDiscoveryMode(QIfConfiguration *config, QIfSettingsObject *so, QIfAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_ASSERT(so);
    if (so->discoveryModeEnvOverride) {
        if (!config || !config->ignoreOverrideWarnings())
            qtif_qmlOrCppWarning(config, "Changing the discoveryMode is not possible, because the QTIF_DISCOVERY_MODE_OVERRIDE env variable has been set.");
        return false;
    }
    so->discoveryMode = discoveryMode;
    so->discoveryModeSet = true;

    for (auto &feature : std::as_const(so->features)) {
        if (!feature)
            continue;
        qCDebug(qLcIfConfig) << "Updating discoveryMode of" << feature << "with" << discoveryMode;
        feature->setDiscoveryMode(so->discoveryMode);
    }
    return true;
}

bool QIfConfigurationManager::setPreferredBackends(QIfConfiguration *config, QIfSettingsObject *so, const QStringList &preferredBackends)
{
    Q_ASSERT(so);
    if (so->preferredBackendsEnvOverride) {
        if (!config || !config->ignoreOverrideWarnings())
            qtif_qmlOrCppWarning(config, "Changing the preferredBackends is not possible, because the QTIF_PREFERRED_BACKENDS_OVERRIDE env variable has been set.");
        return false;
    }
    so->preferredBackends = preferredBackends;
    so->preferredBackendsSet = true;

    for (auto &feature : std::as_const(so->features)) {
        if (!feature)
            continue;
        qCDebug(qLcIfConfig) << "Updating preferredBackends of" << feature << "with" << preferredBackends;
        feature->setPreferredBackends(so->preferredBackends);
    }
    return true;
}

bool QIfConfigurationManager::setServiceObject(QIfSettingsObject *so, QIfServiceObject *serviceObject)
{
    Q_ASSERT(so);
    so->serviceObject = serviceObject;
    so->serviceObjectSet = true;

    for (auto &feature : std::as_const(so->features)) {
        if (!feature)
            continue;
        qCDebug(qLcIfConfig) << "Updating serviceObject of" << feature << "with" << serviceObject;
        feature->setServiceObject(so->serviceObject);
    }
    return true;
}

void QIfConfigurationManager::parseEnv(const QByteArray &rulesSrc, std::function<void(const QString &, const QString &)> func)
{
    const QString content = QString::fromLocal8Bit(rulesSrc);
    const auto lines = content.split(QLatin1Char(';'));
    for (auto line : lines) {
        // Remove whitespace at start and end of line:
        line = line.trimmed();

        int equalPos = line.indexOf(QLatin1Char('='));
        if (equalPos != -1) {
            if (line.lastIndexOf(QLatin1Char('=')) == equalPos) {
                const auto key = line.left(equalPos).trimmed();
                const auto valueStr = line.mid(equalPos + 1).trimmed();

                func(key, valueStr);
            } else {
                qCWarning(qLcIfConfig, "Ignoring malformed override: '%s'", line.toUtf8().constData());
            }
        }
    }
}



QIfConfigurationPrivate::QIfConfigurationPrivate(QIfConfiguration *parent)
    : q_ptr(parent)
    , m_ignoreOverrideWarnings(false)
    , m_settingsObject(nullptr)
    , m_qmlCreation(false)
{
}


/*!
    \class QIfConfiguration
    \inmodule QtInterfaceFramework
    \since 6.5
    \brief QIfConfiguration provides settings for QIfAbstractFeature, QIfServiceObject and
    QIfSimulationEngine.

    QIfConfiguration provides settings for \l QIfAbstractFeature, \l QIfServiceObject and
    \l QIfSimulationEngine.
    All settings configured with QIfConfiguration are applied to all objects with a matching
    \c configurationId. For \l QIfSimulationEngine the identifier acts as \c configurationId.

    Once a new instance of any of the supported classes is created, and its \c configurationId matches
    with a configuration which has been created before, all settings within the configuration are
    also applied to the new instance.

    \note Reading values from QIfConfiguration does NOT read the current values of all instances
    matching the \c configurationId. It only returns the value stored in the configuration, which can
    be different, as it is still possible to change values directly without involving QIfConfiguration.
    It acts as a WRITE ONLY interface towards all matching instances.

    The following snippet shows how a configuration can be created:

    \code
    // Create a class based on QIfAbstractFeature and set it's configuration id to "group1"
    auto feature1 = new AbstractFeatureBasedClass;
    feature1.setConfigurationId("group1");

    // Create another one, but with configuration id "group2"
    auto feature2 = new AbstractFeatureBasedClass;
    feature2.setConfigurationId("group2");

    // The discoveryMode of all feature based instances in "group1" should be LoadOnlyProductionBackends
    QIfConfiguration::setDiscoveryMode("group1", QIfAbstractFeature::LoadOnlyProductionBackends);

    qDebug() << feature1.discoveryMode() << feature2.discoveryMode();
    \endcode

    The configuration is only applied to 'feature1' and NOT to 'feature2', because the latter is
    NOT in 'group1' and 'group2' doesn't have any configured value (yet).

    By adding an additional configuration the feature2 can be changed as well:

    \code
    QIfConfiguration config("group2");
    config.setDiscoeryMode(QIfAbstractFeature::LoadOnlyProductionBackends);

    qDebug() << feature1.discoveryMode() << feature2.discoveryMode();
    \endcode

    Now 'feature1' and 'feature2' have the same discoveryMode. Using an instance of QIfConfiguration
    is an alternative way instead of using the static functions.

    \note Destroying a QIfConfiguration doesn't reset the configured values.

    \section1 QML integration

    A configuration can be created from QML by instantiating the \l InterfaceFrameworkConfiguration
    element and assigning a name to it. Without a name the configuration is not valid and the
    assigned values will have no effect.

    \section1 Settings file

    In addition to creating configurations at runtime, it is also possible to provide the initial
    configuration values in a configuration file.

    If it exists, the following file is parsed at startup:

    [QLibraryInfo::DataPath]/qtifconfig.ini

    The configuration file uses the INI format and is applicable for all types which can also be
    saved as a string. This excludes the \l serviceObject setting for example. See
    \l{Settings Overview} for a list of all settings and how they can be set/overwritten.

    Following an example, which configures group1 and group2:

    \badcode
    [group1]
    preferredBackends=backend1,backend2
    serviceSettings/key1=value1
    serviceSettings/key2=value2
    [group2]
    discoveryMode=LoadOnlySimulationBackends
    \endcode

    For the \l serviceSettings setting, multiple values cannot be assigned in one statement. Instead
    every key-value pair needs to be assigned separately. The key follows the \c serviceSettings keyword
    followed by a slash as separator.
    In a similar fashion, nested key-value pairs can be created:

    \badcode
    [group1]
    serviceSettings/key1/nested1=value1
    serviceSettings/key1/nested2=value2
    \endcode

    \section1 Environment Overrides

    For testing scenarios it is sometimes useful to overwrite certain settings. This can be done
    by using one of the \c OVERRIDE environment variables. Once an override has been set, the value
    cannot be changed at runtime anymore and a warning will be shown when trying to change it.
    See the \l{ignoreOverrideWarnings} property on how this can be turned off.

    All \c OVERRIDE environment variables always have to be in the following form:

    \badcode
    OVERRIDE=<CONFIGURATIONID>=<VALUE>[;<CONFIGURATIONID>=<VALUE>]
    \endcode

    See \l{Settings Overview} for a list of all settings and how they can be set/overwritten.

    \section1 Settings Overview

    \table
        \header
            \li Setting
            \li Affected Classes
            \li Supports Initial Values
            \li Override Environment Variable
        \row
            \li \l serviceSettings
            \li \l QIfServiceObject
            \li yes
            \li -
        \row
            \li \l simulationFile
            \li \l QIfSimulationEngine
            \li yes
            \li QTIF_SIMULATION_OVERRIDE
        \row
            \li \l simulationDataFile
            \li \l QIfSimulationEngine
            \li yes
            \li QTIF_SIMULATION_DATA_OVERRIDE
        \row
            \li \l discoveryMode
            \li \l QIfAbstractFeature \l QIfAbstractFeatureListModel
            \li yes
            \li QTIF_DISCOVERY_MODE_OVERRIDE
        \row
            \li \l preferredBackends
            \li \l QIfAbstractFeature \l QIfAbstractFeatureListModel
            \li yes
            \li QTIF_PREFERRED_BACKENDS_OVERRIDE
        \row
            \li \l serviceObject
            \li \l QIfAbstractFeature \l QIfAbstractFeatureListModel
            \li no
            \li -
    \endtable
*/

/*!
    \qmltype InterfaceFrameworkConfiguration
    \instantiates QIfConfiguration
    \inqmlmodule QtInterfaceFramework
    \since 6.5

    \brief InterfaceFrameworkConfiguration is the QML version of \l QIfConfiguration.

    InterfaceFrameworkConfiguration provides settings for \l QIfAbstractFeature,
    \l QIfServiceObject and \l QIfSimulationEngine.
    All settings configured with InterfaceFrameworkConfiguration are applied to all objects with a
    matching \c configurationId. For \l QIfSimulationEngine the identifier acts as \c configurationId.

    Once a new instance of any of the supported classes is created, and its \c configurationId matches
    with a configuration which has been created before, all settings within the configuration are
    also applied to the new instance.

    \note Reading values from InterfaceFrameworkConfiguration does NOT read the current values of
    all instances matching the \c configurationId. It only returns the value stored in the
    configuration, which can be different, as it is still possible to change values directly without
    involving InterfaceFrameworkConfiguration.
    It acts as a WRITE ONLY interface towards all matching instances.

    The following snippet shows how a configuration can be created:

    \code
    Item {
        AbstractFeatureBasedItem {
            id: feature
            configurationId: "group1"
        }

        InterfaceFrameworkConfiguration {
            name: "group1"
            discoveryMode: AbstractFeature.LoadOnlyProductionBackends
        }

        Component.onCompleted: {
            console.log("DiscoveryMode:", feature.discoveryMode)
        }
    }
    \endcode

    It is also possible to provide settings to the current backend of an interface using the
    \c serviceSettings property. In the following example, the \c connectionUrl of a QtRO backend
    is set for all backends inside the \c cluster group.

    \code
    Item {
        InterfaceFrameworkConfiguration {
            name: "cluster"
            serviceSettings: {
                connectionUrl: "tcp://127.0.0.1:1234"
            }
        }
    }
    \endcode

    See \l{QIfConfiguration} for more information how to provide initial settings and overrides.
*/

/*!
    Constructs a QIfConfiguration instance with a \a name and a \a parent
*/
QIfConfiguration::QIfConfiguration(const QString &name, QObject *parent)
    : QObject(*new QIfConfigurationPrivate(this), parent)
{
    if (!name.isNull())
        setName(name);
}

QIfConfiguration::~QIfConfiguration()
{
    Q_D(const QIfConfiguration);
    if (!d->m_name.isEmpty())
        QIfConfigurationManager::instance()->m_configurationHash.remove(d->m_name);
}

/*!
    \qmlproperty boolean InterfaceFrameworkConfiguration::valid

    Returns \c true when the configuration instance is valid.

    A configuration is only valid if it was created with a name.
*/
/*!
    \property QIfConfiguration::valid

    Returns \c true when the configuration instance is valid.

    A configuration is only valid if it was created with a name.
*/
bool QIfConfiguration::isValid() const
{
    Q_D(const QIfConfiguration);
    return d->m_settingsObject != nullptr;
}

/*!
    \qmlproperty boolean InterfaceFrameworkConfiguration::ignoreOverrideWarnings

    When enabled, all override warnings will be ignored and not logged.

    \sa {Environment Overrides}
*/
/*!
    \property QIfConfiguration::ignoreOverrideWarnings

    When enabled, all override warnings will be ignored and not logged.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::ignoreOverrideWarnings() const
{
    Q_D(const QIfConfiguration);
    return d->m_ignoreOverrideWarnings;
}

/*!
    \qmlproperty string InterfaceFrameworkConfiguration::name

    Holds the name of the configuration. The name is used to find objects with a matching
    configurationId in order to apply settings to them.

    \note Once a name has been set, it cannot be changed afterwards.
*/
/*!
    \property QIfConfiguration::name

    Holds the name of the configuration. The name is used to find objects with a matching
    configurationId in order to apply settings to them.

    \note Once a name has been set, it cannot be changed afterwards.
*/
QString QIfConfiguration::name() const
{
    Q_D(const QIfConfiguration);
    return d->m_name;
}

/*!
    \qmlproperty object InterfaceFrameworkConfiguration::serviceSettings

    Holds the serviceSettings of the configuration. The serviceSettings are applied to all
    QIfServiceObject instances with a matching configurationId.
    The serviceSettings are applied when a new matching QIfServiceObject instance is created and
    they are also applied to all existing QIfServiceObject instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa ServiceObject::serviceSettings
*/
/*!
    \property QIfConfiguration::serviceSettings

    Holds the serviceSettings of the configuration. The serviceSettings are applied to all
    QIfServiceObject instances with a matching configurationId.
    The serviceSettings are applied when a new matching QIfServiceObject instance is created and
    they are also applied to all existing QIfServiceObject instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfServiceObject::serviceSettings
*/
QVariantMap QIfConfiguration::serviceSettings() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QVariantMap());

    return d->m_settingsObject->serviceSettings;
}

/*!
    \qmlproperty string InterfaceFrameworkConfiguration::simulationFile

    Holds the simulationFile of the configuration. The simulationFile is set as override for all
    matching QIfSimulationEngine instances.

    If the matching QIfSimulationeEngine is already running, updating the value doesn't have any
    effect and the simulation will continue to run as it is.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfSimulationEngine::loadSimulation()
*/
/*!
    \property QIfConfiguration::simulationFile

    Holds the simulationFile of the configuration. The simulationFile is set as override for all
    matching QIfSimulationEngine instances.

    If the matching QIfSimulationeEngine is already running, updating the value doesn't have any
    effect and the simulation will continue to run as it is.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfSimulationEngine::loadSimulation()
*/
QString QIfConfiguration::simulationFile() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QString());

    return d->m_settingsObject->simulationFile;
}

/*!
    \qmlproperty string InterfaceFrameworkConfiguration::simulationDataFile

    Holds the simulationDataFile of the configuration. The simulationDataFile is set as override
    for all matching QIfSimulationEngine instances.

    If the matching QIfSimulationeEngine is already running, updating the value doesn't have any
    effect and the simulation will continue to run as it is.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfSimulationEngine::loadSimulationData()
*/
/*!
    \property QIfConfiguration::simulationDataFile

    Holds the simulationDataFile of the configuration. The simulationDataFile is set as override
    for all matching QIfSimulationEngine instances.

    If the matching QIfSimulationeEngine is already running, updating the value doesn't have any
    effect and the simulation will continue to run as it is.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfSimulationEngine::loadSimulationData()
*/
QString QIfConfiguration::simulationDataFile() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QString());

    return d->m_settingsObject->simulationDataFile;
}

/*!
    \qmlproperty enumeration InterfaceFrameworkConfiguration::discoveryMode

    Holds the discoveryMode of the configuration. The discoveryMode is applied to all
    AbstractFeature or AbstractFeatureListModel instances with a matching configurationId.
    The discoveryMode is applied when a new matching instance is created and
    they are also applied to all existing instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa AbstractFeature::discoveryMode AbstractFeatureListModel::discoveryMode
*/
/*!
    \property QIfConfiguration::discoveryMode

    Holds the discoveryMode of the configuration. The discoveryMode is applied to all
    QIfAbstractFeature or QIfAbstractFeatureListModel instances with a matching configurationId.
    The discoveryMode is applied when a new matching instance is created and
    they are also applied to all existing instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfAbstractFeature::discoveryMode QIfAbstractFeatureListModel::discoveryMode
*/
QIfAbstractFeature::DiscoveryMode QIfConfiguration::discoveryMode() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QIfAbstractFeature::DiscoveryMode());

    return d->m_settingsObject->discoveryMode;
}

/*!
    \qmlproperty list<string> InterfaceFrameworkConfiguration::preferredBackends

    Holds the preferredBackends of the configuration. The preferredBackends are applied to all
    AbstractFeature or AbstractFeatureListModel instances with a matching configurationId.
    The preferredBackends are applied when a new matching instance is created and
    it is also applied to all existing instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa AbstractFeature::preferredBackends AbstractFeatureListModel::preferredBackends
*/
/*!
    \property QIfConfiguration::preferredBackends

    Holds the preferredBackends of the configuration. The preferredBackends are applied to all
    QIfAbstractFeature or QIfAbstractFeatureListModel instances with a matching configurationId.
    The preferredBackends are applied when a new matching instance is created and
    it is also applied to all existing instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfAbstractFeature::preferredBackends QIfAbstractFeatureListModel::preferredBackends
*/
QStringList QIfConfiguration::preferredBackends() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QStringList());

    return d->m_settingsObject->preferredBackends;
}

/*!
    \qmlproperty ServiceObject InterfaceFrameworkConfiguration::serviceObject

    Holds the serviceObject of the configuration. The serviceObject is applied to all
    AbstractFeature or AbstractFeatureListModel instances with a matching configurationId.
    The serviceObject is applied when a new matching instance is created and
    it is also applied to all existing instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa AbstractFeature::serviceObject AbstractFeatureListModel::serviceObject
*/
/*!
    \property QIfConfiguration::serviceObject

    Holds the serviceObject of the configuration. The serviceObject is applied to all
    QIfAbstractFeature or QIfAbstractFeatureListModel instances with a matching configurationId.
    The serviceObject is applied when a new matching instance is created and
    it is also applied to all existing instances.

    See \l{Settings Overview} for how to provide initial values and overrides.

    \sa QIfAbstractFeature::serviceObject QIfAbstractFeatureListModel::serviceObject
*/
QIfServiceObject *QIfConfiguration::serviceObject() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(nullptr);

    return d->m_settingsObject->serviceObject;
}

void QIfConfiguration::setIgnoreOverrideWarnings(bool ignoreOverrideWarnings)
{
    Q_D(QIfConfiguration);

    if (d->m_ignoreOverrideWarnings == ignoreOverrideWarnings)
        return;

    d->m_ignoreOverrideWarnings = ignoreOverrideWarnings;
    emit ignoreOverrideWarningsChanged(ignoreOverrideWarnings);
}

/*!
    Sets the \a name of the configuration.

    The name is used to find objects with a matching
    configurationId in order to apply all settings of this configuration to them.

    Returns \c false if setting the name failed e.g. when a instance with this name already exists or
    when the instance already has a name set. The name of an instance can't be changed once it has
    been set. Returns \c true otherwise.
*/
bool QIfConfiguration::setName(const QString &name)
{
    Q_D(QIfConfiguration);

    if (name.isEmpty())
        return false;

    // Skip all checks during QML creation
    // All values will be stored in a temporary SettingsObject until componentComplete is called
    if (d->m_qmlCreation) {
        d->m_name = name;
        emit nameChanged(name);
        emit isValidChanged(true);
        return true;
    }

    if (d->m_settingsObject) {
        qtif_qmlOrCppWarning(this, "The name of the Configuration Object can't be changed once it has been set.");
        return false;
    }

    if (QIfConfigurationManager::instance()->m_configurationHash.contains(name)) {
        qtif_qmlOrCppWarning(this, "A Configuration Object with this name already exists.");
        return false;
    }

    d->m_name = name;
    d->m_settingsObject = QIfConfigurationManager::instance()->settingsObject(name, true);

    QIfConfigurationManager::instance()->m_configurationHash.insert(name, this);

    emit nameChanged(name);
    emit isValidChanged(true);

    return true;
}

/*!
    Sets the \a serviceSettings of this configuration and applies it to all QIfServiceObject instances
    with a matching configurationId.

    Returns \c false if setting the serviceObject failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setServiceSettings(const QVariantMap &serviceSettings)
{
    Q_D(QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(false);

    if (d->m_settingsObject->serviceSettings == serviceSettings)
        return false;

    if (QIfConfigurationManager::instance()->setServiceSettings(d->m_settingsObject, serviceSettings)) {
        emit serviceSettingsChanged(serviceSettings);
        return true;
    }

    return false;
}

/*!
    Sets the \a simulationFile of this configuration and applies it to all QIfSimulationEngine instances
    with a matching configurationId.

    Returns \c false if setting the simulationFile failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setSimulationFile(const QString &simulationFile)
{
    Q_D(QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(false);

    if (d->m_settingsObject->simulationFile == simulationFile)
        return false;

    if (QIfConfigurationManager::instance()->setSimulationFile(this, d->m_settingsObject, simulationFile)) {
        emit simulationFileChanged(simulationFile);
        return true;
    }

    return false;
}

/*!
    Sets the \a simulationDataFile of this configuration and applies it to all QIfSimulationEngine instances
    with a matching configurationId.

    Returns \c false if setting the simulationDataFile failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setSimulationDataFile(const QString &simulationDataFile)
{
    Q_D(QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(false);

    if (d->m_settingsObject->simulationDataFile == simulationDataFile)
        return false;

    if (QIfConfigurationManager::instance()->setSimulationDataFile(this, d->m_settingsObject, simulationDataFile)) {
        emit simulationDataFileChanged(simulationDataFile);
        return true;
    }

    return false;
}

/*!
    Sets the \a discoveryMode of this configuration and applies it to all QIfAbstractFeature or
    QIfAbstractFeatureListModel instances with a matching configurationId.

    Returns \c false if setting the discoveryMode failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setDiscoveryMode(QIfAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(false);

    if (d->m_settingsObject->discoveryMode == discoveryMode)
        return false;

    if (QIfConfigurationManager::instance()->setDiscoveryMode(this, d->m_settingsObject, discoveryMode)) {
        emit discoveryModeChanged(discoveryMode);
        return true;
    }

    return false;
}

/*!
    Sets the \a preferredBackends of this configuration and applies it to all QIfAbstractFeature or
    QIfAbstractFeatureListModel instances with a matching configurationId.

    Returns \c false if setting the preferredBackends failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setPreferredBackends(const QStringList &preferredBackends)
{
    Q_D(QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(false);

    if (d->m_settingsObject->preferredBackends == preferredBackends)
        return false;

    if (QIfConfigurationManager::instance()->setPreferredBackends(this, d->m_settingsObject, preferredBackends)) {
        emit preferredBackendsChanged(preferredBackends);
        return true;
    }

    return false;
}

/*!
    Sets the \a serviceObject of this configuration and applies it to all QIfAbstractFeature or
    QIfAbstractFeatureListModel instances with a matching configurationId.

    Returns \c false if setting the serviceObject failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setServiceObject(QIfServiceObject *serviceObject)
{
    Q_D(QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(false);

    if (d->m_settingsObject->serviceObject == serviceObject)
        return false;

    if (QIfConfigurationManager::instance()->setServiceObject(d->m_settingsObject, serviceObject)) {
        emit serviceObjectChanged(serviceObject);
        return true;
    }

    return false;
}

void QIfConfiguration::classBegin()
{
    Q_D(QIfConfiguration);
    d->m_qmlCreation = true;
    // storage for all settings until we can resolve the real settingsobject in compoentComplete
    d->m_settingsObject = new QIfSettingsObject();
}

void QIfConfiguration::componentComplete()
{
    Q_D(QIfConfiguration);

    d->m_qmlCreation = false;

    QScopedPointer<QIfSettingsObject> tempSettings(d->m_settingsObject);
    d->m_settingsObject = nullptr;
    // Resolve the real settingsObject;
    if (!setName(d->m_name))
        return;

    Q_CHECK_SETTINGSOBJECT(void());

    //Copy all values from the tempObject
    setServiceSettings(tempSettings->serviceSettings);
    setSimulationFile(tempSettings->simulationFile);
    setSimulationDataFile(tempSettings->simulationDataFile);
    setPreferredBackends(tempSettings->preferredBackends);
    setDiscoveryMode(tempSettings->discoveryMode);
    setServiceObject(tempSettings->serviceObject);
}

QIfConfiguration::QIfConfiguration(QIfConfigurationPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

// static member

/*!
    Returns \c true if the configuration \a group exists.

    A configuration is created once a single value has been set in it.
*/
bool QIfConfiguration::exists(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so != nullptr;
}

/*!
    Returns the current service settings of the configuration \a group.

    \note The returned value is what is stored inside the configuration, not the current value of all
    QIfServiceObject instances with a matching configurationId.
*/
QVariantMap QIfConfiguration::serviceSettings(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceSettings : QVariantMap();
}

/*!
    Sets the \a serviceSettings of the configuration \a group and applies it to all QIfServiceObject
    instances with a matching configurationId.

    Returns \c false if setting the serviceObject failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setServiceSettings(const QString &group, const QVariantMap &serviceSettings)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setServiceSettings(so, serviceSettings);
}

/*!
    Returns \c true when service settings have been set in the configuration named \a group and
    false otherwise.

    A value is considered as "set" when the corresponding setter was called, a valid value was set
    in the global ini file or the corresponding override is active.

    \sa {Settings file}
*/
bool QIfConfiguration::areServiceSettingsSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceSettingsSet : false;
}

/*!
    Returns the current simulation file of the configuration \a group.

    \note The returned value is what is stored inside the configuration, not the current value of all
    QIfSimulationEngine instances with a matching configurationId.
*/
QString QIfConfiguration::simulationFile(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationFile : QString();
}

/*!
    Sets the \a simulationFile of the configuration \a group and applies it to all
    QIfSimulationEngine instances with a matching configurationId.

    Returns \c false if setting the simulationDataFile failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setSimulationFile(const QString &group, const QString &simulationFile)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setSimulationFile(nullptr, so, simulationFile);
}

/*!
    Returns \c true when the simulation file have been set in the configuration named \a group and
    false otherwise.

    A value is considered as "set" when the corresponding setter was called, a valid value was set
    in the global ini file or the corresponding override is active.

    \sa {Settings file}
*/
bool QIfConfiguration::isSimulationFileSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationFileSet : false;
}

/*!
    Returns the current simulation data file of the configuration \a group.

    \note The returned value is what is stored inside the configuration, not the current value of all
    QIfSimulationEngine instances with a matching configurationId.
*/
QString QIfConfiguration::simulationDataFile(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationDataFile : QString();
}

/*!
    Sets the \a simulationDataFile of the configuration \a group and applies it to all
    QIfSimulationEngine instances with a matching configurationId.

    Returns \c false if setting the simulationDataFile failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setSimulationDataFile(const QString &group, const QString &simulationDataFile)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setSimulationDataFile(nullptr, so, simulationDataFile);
}

/*!
    Returns \c true when the simulation data file have been set in the configuration named \a group and
    false otherwise.

    A value is considered as "set" when the corresponding setter was called, a valid value was set
    in the global ini file or the corresponding override is active.

    \sa {Settings file}
*/
bool QIfConfiguration::isSimulationDataFileSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationDataFileSet : false;
}

/*!
    Returns the current discovery mode of the configuration \a group.

    \note The returned value is what is stored inside the configuration, not the current value of all
    QIfAbstractFeature or QIfAbstractFeatureListModel instances with a matching configurationId.
*/
QIfAbstractFeature::DiscoveryMode QIfConfiguration::discoveryMode(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->discoveryMode : QIfAbstractFeature::InvalidAutoDiscovery;
}

/*!
    Sets the \a discoveryMode of the configuration \a group and applies it to all QIfAbstractFeature or
    QIfAbstractFeatureListModel instances with a matching configurationId.

    Returns \c false if setting the serviceObject failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setDiscoveryMode(const QString &group, QIfAbstractFeature::DiscoveryMode discoveryMode)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setDiscoveryMode(nullptr, so, discoveryMode);
}

/*!
    Returns \c true when the discovery mode have been set in the configuration named \a group and
    false otherwise.

    A value is considered as "set" when the corresponding setter was called, a valid value was set
    in the global ini file or the corresponding override is active.

    \sa {Settings file}
*/
bool QIfConfiguration::isDiscoveryModeSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->discoveryModeSet : false;
}

/*!
    Returns the current preferred backends of the configuration \a group.

    \note The returned value is what is stored inside the configuration, not the current value of all
    QIfAbstractFeature or QIfAbstractFeatureListModel instances with a matching configurationId.
*/
QStringList QIfConfiguration::preferredBackends(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->preferredBackends : QStringList();
}

/*!
    Sets the \a preferredBackends of the configuration \a group and applies it to all QIfAbstractFeature or
    QIfAbstractFeatureListModel instances with a matching configurationId.

    Returns \c false if setting the serviceObject failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setPreferredBackends(const QString &group, const QStringList &preferredBackends)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setPreferredBackends(nullptr, so, preferredBackends);
}

/*!
    Returns \c true when the preferred backends have been set in the configuration named \a group and
    false otherwise.

    A value is considered as "set" when the corresponding setter was called, a valid value was set
    in the global ini file or the corresponding override is active.

    \sa {Settings file}
*/
bool QIfConfiguration::arePreferredBackendsSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->preferredBackendsSet : false;
}

/*!
    Returns the current service object of the configuration \a group.

    \note The returned value is what is stored inside the configuration, not the current value of all
    QIfAbstractFeature or QIfAbstractFeatureListModel instances with a matching configurationId.
*/
QIfServiceObject *QIfConfiguration::serviceObject(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceObject : nullptr;
}

/*!
    Sets the \a serviceObject of the configuration \a group and applies it to all QIfAbstractFeature or
    QIfAbstractFeatureListModel instances with a matching configurationId.

    Returns \c false if setting the serviceObject failed because an override was active, returns \c true
    otherwise.

    \sa {Environment Overrides}
*/
bool QIfConfiguration::setServiceObject(const QString &group, QIfServiceObject *serviceObject)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setServiceObject(so, serviceObject);
}

/*!
    Returns \c true when the service object have been set in the configuration named \a group and
    false otherwise.

    A value is considered as "set" when the corresponding setter was called, a valid value was set
    in the global ini file or the corresponding override is active.

    \sa {Settings file}
*/
bool QIfConfiguration::isServiceObjectSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceObjectSet : false;
}


QT_END_NAMESPACE

#include "moc_qifconfiguration.cpp"
