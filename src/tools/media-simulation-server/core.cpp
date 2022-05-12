/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

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
