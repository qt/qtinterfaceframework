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
    QVariantMap serviceSettings;
    bool serviceSettingsSet = false;
    QList<QPointer<QIfProxyServiceObject>> serviceObjects;
    QList<QPointer<QIfAbstractFeature>> features;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfConfigurationManager
{
public:
    static QIfConfigurationManager *instance();

    void readInitialSettings();
    QIfSettingsObject *settingsObject(const QString &group, bool create = false);

    void addServiceObject(const QString &group, QIfProxyServiceObject *serviceObject);
    void removeServiceObject(const QString &group, QIfProxyServiceObject *serviceObject);
    void addAbstractFeature(const QString &group, QIfAbstractFeature *feature);
    void removeAbstractFeature(const QString &group, QIfAbstractFeature *feature);
    bool setServiceSettings(QIfSettingsObject *so, const QVariantMap &serviceSettings);
    bool setSimulationFile(QIfSettingsObject *so, const QString &simulationFile);
    bool setSimulationDataFile(QIfSettingsObject *so, const QString &simulationDataFile);
    bool setDiscoveryMode(QIfSettingsObject *so, QIfAbstractFeature::DiscoveryMode discoveryMode);

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
    QIfSettingsObject *m_settingsObject;
};

QT_END_NAMESPACE

#endif // QIFCONFIGURATION_P_H

