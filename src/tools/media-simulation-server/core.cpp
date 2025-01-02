// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "core.h"
#include <QCoreApplication>
#include <QSettings>

QT_BEGIN_NAMESPACE

Core* Core::s_instance(nullptr);

Core::Core(QObject *parent)
    : QObject(parent)
    , m_host(nullptr)
{
    init();
}

Core::~Core()
{
}

void Core::init()
{
    QString configPath(QStringLiteral("./server.conf"));
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH"))
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
    else
        qDebug() << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("qtifmedia"));
    QUrl url = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:qtifmedia")).toString());
    m_host = new QRemoteObjectRegistryHost(url);
    qDebug() << "registry at: " << m_host->registryUrl().toString();
    connect(m_host, &QRemoteObjectNode::error, this, &Core::reportError);
}

Core* Core::instance()
{
    if (!s_instance)
        s_instance = new Core(QCoreApplication::instance());
    Q_ASSERT(s_instance);
    return s_instance;
}

QRemoteObjectRegistryHost* Core::host() const
{
    Q_ASSERT(m_host);
    return m_host;
}

void Core::reportError(QRemoteObjectNode::ErrorCode code)
{
    qWarning() << "QRemoteObjects Error: " << code;
}

QT_END_NAMESPACE
