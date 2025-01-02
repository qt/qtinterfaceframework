// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFCONFIGURATION_P_H
#define QIFCONFIGURATION_P_H

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

#include "qifconfiguration.h"
#include "qifproxyserviceobject.h"

#include <QtCore/QLoggingCategory>
#include <QtCore/QSettings>

#include <private/qobject_p.h>
#include <private/qtifglobal_p.h>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcIfConfig)

class QIfSettingsObject {
public:
    QString simulationDataFile;
    bool simulationDataFileSet = false;
    bool simulationDataFileEnvOverride = false;
    QString simulationFile;
    bool simulationFileSet = false;
    bool simulationFileEnvOverride = false;
    QIfAbstractFeature::DiscoveryMode discoveryMode = QIfAbstractFeature::InvalidAutoDiscovery;
    bool discoveryModeSet = false;
    bool discoveryModeEnvOverride = false;
    QStringList preferredBackends;
    bool preferredBackendsSet = false;
    bool preferredBackendsEnvOverride = false;
    QPointer<QIfServiceObject> serviceObject;
    bool serviceObjectSet = false;
    QVariantMap serviceSettings;
    bool serviceSettingsSet = false;
    QList<QPointer<QIfProxyServiceObject>> serviceObjects;
    QList<QPointer<QIfAbstractFeature>> features;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfConfigurationManager
{
public:
    static QIfConfigurationManager *instance();

    void readInitialSettings(const QString &configPath);
    QIfSettingsObject *settingsObject(const QString &group, bool create = false);

    void addServiceObject(const QString &group, QIfProxyServiceObject *serviceObject);
    void removeServiceObject(const QString &group, QIfProxyServiceObject *serviceObject);
    void addAbstractFeature(const QString &group, QIfAbstractFeature *feature);
    void removeAbstractFeature(const QString &group, QIfAbstractFeature *feature);
    bool setServiceSettings(QIfSettingsObject *so, const QVariantMap &serviceSettings);
    bool setSimulationFile(QIfConfiguration *obj, QIfSettingsObject *so, const QString &simulationFile);
    bool setSimulationDataFile(QIfConfiguration *obj, QIfSettingsObject *so, const QString &simulationDataFile);
    bool setDiscoveryMode(QIfConfiguration *obj, QIfSettingsObject *so, QIfAbstractFeature::DiscoveryMode discoveryMode);
    bool setPreferredBackends(QIfConfiguration *obj, QIfSettingsObject *so, const QStringList &preferredBackends);
    bool setServiceObject(QIfSettingsObject *so, QIfServiceObject *serviceObject);

    QVariantMap readGroup(QSettings *settings, QAnyStringView group);
    void parseEnv(const QByteArray &rulesSrc, std::function<void(const QString &, const QString &)> func);

    QHash<QString, QIfSettingsObject*> m_settingsHash;
    QHash<QString, QIfConfiguration*> m_configurationHash;
private:
    QIfConfigurationManager();
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfConfigurationPrivate : public QObjectPrivate
{
public:
    QIfConfigurationPrivate(QIfConfiguration *parent);

    QIfConfiguration * const q_ptr;
    Q_DECLARE_PUBLIC(QIfConfiguration)
    Q_DISABLE_COPY(QIfConfigurationPrivate)

    QString m_name;
    bool m_ignoreOverrideWarnings;
    QIfSettingsObject *m_settingsObject;
    bool m_qmlCreation;
};

QT_END_NAMESPACE

#endif // QIFCONFIGURATION_P_H

