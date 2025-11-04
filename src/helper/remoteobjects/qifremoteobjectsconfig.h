// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFREMOTEOBJECTSCONFIG_H
#define QIFREMOTEOBJECTSCONFIG_H

#include <QtCore/QUrl>
#include <QtCore/QSettings>
#include <QtRemoteObjects/QRemoteObjectHost>

#include "QtIfRemoteObjectsHelper/qtifremoteobjectshelper_global.h"

QT_BEGIN_NAMESPACE

class Q_IFREMOTEOBJECTSHELPER_EXPORT QIfRemoteObjectsConfig
{
public:
    QIfRemoteObjectsConfig() = default;
    virtual ~QIfRemoteObjectsConfig() = default;

    void setDefaultServerUrl(const QUrl &defaultServerUrl);
    void parseConfigFile(const QString &confFilePath);
    void parseLegacyConfigFile();

    void setReportErrorsOnStdErr(bool enabled);
    bool reportErrorsOnStdErr() const;

    QRemoteObjectHost *host(const QString &module, const QString &interface, const QUrl &fallbackUrl = QUrl());
    QRemoteObjectHost *host(const QUrl &url);

    // pass object as the first argument to be similar to the qtro enableRemoting ?
    bool enableRemoting(const QString &module, const QString &interface, const QUrl &fallbackUrl, QObject *object);
    bool enableRemoting(const QString &module, const QString &interface, QObject *object);

private:
    QUrl m_defaultServer;
    std::unique_ptr<QSettings> m_settings;
    bool m_reportErrorsOnStdErr = true;
    QHash<QUrl, QRemoteObjectHost *> m_hostHash;

    Q_DISABLE_COPY_MOVE(QIfRemoteObjectsConfig)
};

QT_END_NAMESPACE

#endif // QIFREMOTEOBJECTSCONFIG_H
