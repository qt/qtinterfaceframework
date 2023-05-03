{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = "Core" %}


#include "{{class|lower}}.h"
#include "{{module.module_name|lower}}.h"
#include <QCoreApplication>
#include <QSettings>

#include <QtIfRemoteObjectsHelper/qifremoteobjectshelper.h>

using namespace Qt::StringLiterals;

{{ module|begin_namespace }}

{{class}}* {{class}}::s_instance(nullptr);

{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
    , m_host(nullptr)
{
    init();
}

{{class}}::~{{class}}()
{
}

void {{class}}::init()
{
    {{module.module_name|upperfirst}}::registerTypes();

    QString configPath(u"./server.conf"_s);
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH"))
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
    else
        qDebug() << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(u"{{module.module_name|lower}}"_s);
    QUrl url = QUrl(settings.value(u"Registry"_s, QIfRemoteObjectsHelper::buildDefaultUrl(u"{{module.module_name|lower}}"_s)).toString());
    m_host = new QRemoteObjectRegistryHost(url);
    qDebug() << "registry at: " << m_host->registryUrl().toString();
    connect(m_host, &QRemoteObjectNode::error, this, &{{class}}::reportError);
}

{{class}}* {{class}}::instance()
{
    if (!s_instance)
        s_instance = new {{class}}(QCoreApplication::instance());
    Q_ASSERT(s_instance);
    return s_instance;
}

QRemoteObjectRegistryHost* {{class}}::host() const
{
    Q_ASSERT(m_host);
    return m_host;
}

void {{class}}::reportError(QRemoteObjectNode::ErrorCode code)
{
    qWarning() << "QRemoteObjects Error: " << code;
}

{{ module|end_namespace }}
