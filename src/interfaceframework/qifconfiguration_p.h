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
    bool serviceSettingsSet = false;
    QVariantMap serviceSettings;
    QList<QPointer<QIfProxyServiceObject>> serviceObjects;
};

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfConfigurationManager
{
public:
    static QIfConfigurationManager *instance();

    void readInitialSettings();
    QIfSettingsObject *settingsObject(const QString &group, bool create = false);

    void addServiceObject(const QString &group, QIfProxyServiceObject *serviceObject);
    void removeServiceObject(const QString &group, QIfProxyServiceObject *serviceObject);
    bool setServiceSettings(QIfSettingsObject *so, const QVariantMap &serviceSettings);

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

