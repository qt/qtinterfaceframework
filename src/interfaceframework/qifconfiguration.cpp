// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifconfiguration.h"
#include "qifconfiguration_p.h"

#include "qifqmlconversion_helper.h"

#include <QLibraryInfo>
#include <QSettings>
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
    readInitialSettings();
}

QIfConfigurationManager *QIfConfigurationManager::instance()
{
    static QIfConfigurationManager s_manager;
    return &s_manager;
}

void QIfConfigurationManager::readInitialSettings()
{
    // read file
    const QString configFileName = QStringLiteral("qtifconfig.ini");

    const QString configPath
                = QDir(QLibraryInfo::path(QLibraryInfo::DataPath)).absoluteFilePath(configFileName);

    qCDebug(qLcIfConfig) << "Loading initial settings from " << configPath;

    QSettings settings(configPath, QSettings::IniFormat);

    for (const QString& group : settings.childGroups()) {
        auto settingsObject = new QIfSettingsObject;

        settings.beginGroup(group);
        settingsObject->serviceSettingsSet = settings.contains("serviceSettings");
        settingsObject->serviceSettings = settings.value("serviceSettings").toMap();
        settings.endGroup();

        m_settingsHash.insert(group, settingsObject);
    }
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

bool QIfConfigurationManager::setServiceSettings(QIfSettingsObject *so, const QVariantMap &serviceSettings)
{
    Q_ASSERT(so);
    so->serviceSettings = serviceSettings;
    so->serviceSettingsSet = true;

    for (auto &serviceObject : qAsConst(so->serviceObjects)) {
        if (!serviceObject)
            continue;
        qCDebug(qLcIfConfig) << "Updating Service Settings of" << serviceObject << "with" << serviceSettings;
        serviceObject->updateServiceSettings(serviceSettings);
    }
    return true;
}


QIfConfigurationPrivate::QIfConfigurationPrivate(QIfConfiguration *parent)
    : q_ptr(parent)
    , m_settingsObject(nullptr)
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

bool QIfConfiguration::setName(const QString &name)
{
    Q_D(QIfConfiguration);

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

void QIfConfiguration::classBegin()
{

}

void QIfConfiguration::componentComplete()
{
    Q_D(const QIfConfiguration);

    Q_CHECK_SETTINGSOBJECT(void());
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
    QIfSettingsObject *so = QIfConfigurationManager::instance()->settingsObject(group, true);
    return so ? so->serviceSettingsSet : false;
}

QT_END_NAMESPACE

#include "moc_qifconfiguration.cpp"
