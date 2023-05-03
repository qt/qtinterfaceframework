{#
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}DBusBackend'.format(interface) %}

#include "{{class|lower}}.h"

#include <QDBusConnection>

using namespace Qt::StringLiterals;

{{class}}::{{class}}(QObject *parent)
    : {{interface}}BackendInterface(parent)
    , m_client(nullptr)
{
{% for struct in module.structs %}
    qDBusRegisterMetaType<{{struct}}>();
{% endfor %}
{% for enum in module.enums %}
    qDBusRegisterMetaType<{{module.module_name}}::{{enum|flag_type}}>();
{% endfor %}
}

void {{class}}::initialize()
{
    if (!m_client)
        setupConnection();
    if (m_fetchList.isEmpty()) {
{% for property in interface.properties %}
        emit {{property}}Changed(m_{{property}});
{% endfor %}
        emit initializationDone();
    }
}

void {{class}}::setupConnection()
{
    qInfo() << "Connecting to the Server";
    m_client = new {{interface.tags.config_dbus.className}}(u"{{interface.tags.config_dbus.interfaceName}}"_s, u"/"_s, QDBusConnection::sessionBus());
{% for property in interface.properties %}
    connect(m_client, &{{interface.tags.config_dbus.className}}::{{property}}Changed,
            this, &{{class}}::on{{property|upperfirst}}Changed);
{% endfor %}
{% for property in interface.properties %}
    void fetch{{property|upperfirst}}();
{% endfor %}
}

{% for property in interface.properties %}
void {{class}}::fetch{{property|upperfirst}}()
{
    m_fetchList.append(u"{{property}}"_s);
    auto reply = m_client->asyncCall(u"{{property}}"_s);
    auto watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this](QDBusPendingCallWatcher *watcher) {
        QDBusPendingReply<{{property|return_type}}> reply = *watcher;
        if (reply.isError()) {
            qCritical() << reply.error();
        } else {
            m_fetchList.removeAll(u"{{property}}"_s);
            this->on{{property|upperfirst}}Changed(reply.value());
            watcher->deleteLater();
            this->checkInitDone();
        }
    });
}
{% endfor %}

void {{class}}::checkInitDone()
{
    if (m_fetchList.isEmpty()) {
        qInfo() << "All properties initialized";
        emit initializationDone();
    }
}

{% for property in interface.properties %}
void {{class}}::on{{property|upperfirst}}Changed({{property|parameter_type}})
{
    m_{{property}} = {{property}};
    emit {{property}}Changed({{property}});
}
{% endfor %}


