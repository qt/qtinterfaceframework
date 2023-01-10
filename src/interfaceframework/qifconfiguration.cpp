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
    if (so->simulationFileEnvOverride) {
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

bool QIfConfiguration::isValid() const
{
    Q_D(const QIfConfiguration);
    return d->m_settingsObject != nullptr;
}

bool QIfConfiguration::ignoreOverrideWarnings() const
{
    Q_D(const QIfConfiguration);
    return d->m_ignoreOverrideWarnings;
}

QString QIfConfiguration::name() const
{
    Q_D(const QIfConfiguration);
    return d->m_name;
}

QVariantMap QIfConfiguration::serviceSettings() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QVariantMap());

    return d->m_settingsObject->serviceSettings;
}

QString QIfConfiguration::simulationFile() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QString());

    return d->m_settingsObject->simulationFile;
}

QString QIfConfiguration::simulationDataFile() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QString());

    return d->m_settingsObject->simulationDataFile;
}

QIfAbstractFeature::DiscoveryMode QIfConfiguration::discoveryMode() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QIfAbstractFeature::DiscoveryMode());

    return d->m_settingsObject->discoveryMode;
}

QStringList QIfConfiguration::preferredBackends() const
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(QStringList());

    return d->m_settingsObject->preferredBackends;
}

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

bool QIfConfiguration::exists(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so != nullptr;
}

QVariantMap QIfConfiguration::serviceSettings(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceSettings : QVariantMap();
}

bool QIfConfiguration::setServiceSettings(const QString &group, const QVariantMap &serviceSettings)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setServiceSettings(so, serviceSettings);
}

bool QIfConfiguration::areServiceSettingsSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceSettingsSet : false;
}

QString QIfConfiguration::simulationFile(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationFile : QString();
}

bool QIfConfiguration::setSimulationFile(const QString &group, const QString &simulationFile)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setSimulationFile(nullptr, so, simulationFile);
}

bool QIfConfiguration::isSimulationFileSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationFileSet : false;
}

QString QIfConfiguration::simulationDataFile(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationDataFile : QString();
}

bool QIfConfiguration::setSimulationDataFile(const QString &group, const QString &simulationDataFile)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setSimulationDataFile(nullptr, so, simulationDataFile);
}

bool QIfConfiguration::isSimulationDataFileSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->simulationDataFileSet : false;
}

QIfAbstractFeature::DiscoveryMode QIfConfiguration::discoveryMode(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->discoveryMode : QIfAbstractFeature::InvalidAutoDiscovery;
}

bool QIfConfiguration::setDiscoveryMode(const QString &group, QIfAbstractFeature::DiscoveryMode discoveryMode)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setDiscoveryMode(nullptr, so, discoveryMode);
}

bool QIfConfiguration::isDiscoveryModeSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->discoveryModeSet : false;
}

QStringList QIfConfiguration::preferredBackends(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->preferredBackends : QStringList();
}

bool QIfConfiguration::setPreferredBackends(const QString &group, const QStringList &preferredBackends)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setPreferredBackends(nullptr, so, preferredBackends);
}

bool QIfConfiguration::arePreferredBackendsSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->preferredBackendsSet : false;
}

QIfServiceObject *QIfConfiguration::serviceObject(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceObject : nullptr;
}

bool QIfConfiguration::setServiceObject(const QString &group, QIfServiceObject *serviceObject)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return QIfConfigurationManager::instance()->setServiceObject(so, serviceObject);
}

bool QIfConfiguration::isServiceObjectSet(const QString &group)
{
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group);
    return so ? so->serviceObjectSet : false;
}


QT_END_NAMESPACE

#include "moc_qifconfiguration.cpp"
