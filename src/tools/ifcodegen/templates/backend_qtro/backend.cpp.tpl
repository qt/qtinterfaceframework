{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}RoBackend'.format(interface) %}
{% set zone_class = '{0}RoZone'.format(interface) %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
#include "{{class|lower}}.h"

#include <QDebug>
#include <QSettings>
#include <QTimer>
#include "{{module.module_name|lower}}.h"

using namespace Qt::StringLiterals;

Q_LOGGING_CATEGORY(qLcRO{{interface}}, "{{module|qml_type|lower}}.{{interface|lower}}backend.remoteobjects", QtInfoMsg)

{{ module|begin_namespace }}

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "pagingmodel.cpp.tpl" %}
{%   endif %}
{% endfor %}

{% if interface_zoned %}
{{zone_class}}::{{zone_class}}(const QString &zone, {{class}} *parent)
    : QObject(parent)
    , m_parent(parent)
    , m_zone(zone)
{% for property in interface.properties %}
{%   if property.type.is_model %}
    , m_{{property}}(new Zoned{{property|upperfirst}}RoModelBackend(u"{{interface.qualified_name}}.{{property}}."_s + m_zone, this))
{%   else %}
    , m_{{ property }}({{property|default_type_value}})
{%   endif %}
{% endfor %}
{
}

bool {{zone_class}}::isSyncing()
{
    return !m_propertiesToSync.isEmpty();
}

void {{zone_class}}::sync()
{
    if (m_parent->m_replica.isNull())
        return;

{% for property in interface.properties %}
{%   if not property.type.is_model %}
    m_propertiesToSync.append(u"{{property}}"_s);
{%   endif %}
{% endfor %}

{% for property in interface.properties %}
{%   if not property.type.is_model %}
    QRemoteObjectPendingReply<{{property|return_type}}> {{property}}Reply = m_parent->m_replica->{{property|getter_name}}(m_zone);
    auto {{property}}Watcher = new QRemoteObjectPendingCallWatcher({{property}}Reply, this);
    connect({{property}}Watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
            m_{{property}} = self->returnValue().value<{{property|return_type}}>();
            m_propertiesToSync.removeAll(u"{{property}}"_s);
            checkSync();
        }
        self->deleteLater();
    });
{%   endif %}
{% endfor %}
}

void {{zone_class}}::checkSync()
{
    if (m_propertiesToSync.isEmpty())
        Q_EMIT syncDone();
}

void {{zone_class}}::emitCurrentState()
{
{% for property in interface.properties %}
    emit m_parent->{{property}}Changed(m_{{property}}, m_zone);
{% endfor %}
}

{% for property in interface.properties %}
{{qtif.prop_setter(property, zone_class, model_interface = true)}}
{
    m_{{property}} = {{property}};
    Q_EMIT m_parent->{{property}}Changed({{property}}, m_zone);
}
{% endfor %}
{% endif %}

{{class}}::{{class}}(const QString &remoteObjectsLookupName, QObject *parent)
    : {{interface}}BackendInterface(parent)
    , m_node(nullptr)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_helper(new QIfRemoteObjectsReplicaHelper(qLcRO{{interface}}(), this))
{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     if interface_zoned %}
    , m_{{property}}(new Zoned{{property|upperfirst}}RoModelBackend(u"{{interface.qualified_name}}.{{property}}"_s, this))
{%     else %}
    , m_{{property}}(new {{property|upperfirst}}RoModelBackend(u"{{interface.qualified_name}}.{{property}}"_s, this))
{%     endif %}
{%   endif %}
{% endfor %}
{% if interface_zoned %}
    , m_synced(false)
{% endif %}
{
    {{module.module_name|upperfirst}}::registerTypes();

{% if interface_zoned %}
    auto zoneObject = new {{zone_class}}(QString(), this);
    m_zoneMap.insert(QString(), zoneObject);
    connect(zoneObject, &{{zone_class}}::syncDone, this, &{{class}}::onZoneSyncDone);
{% endif %}
}

{{class}}::~{{class}}()
{
    delete m_node;
}

void {{class}}::initialize()
{
{% for property in interface.properties %}
{%   if property.type.is_model %}
    Q_EMIT {{property}}Changed(m_{{property}});
{%   endif %}
{% endfor %}

    if (!connectToNode())
        return;

{% if interface_zoned %}
    if (m_synced)
        onZoneSyncDone();
{% else %}
    if (m_replica->isInitialized()) {
{%   for property in interface.properties %}
{%     if not property.is_model %}
        Q_EMIT {{property}}Changed(m_replica->{{property}}());
{%     endif %}
{%   endfor %}
        Q_EMIT initializationDone();
    }
{% endif %}
}

void {{class}}::updateServiceSettings(const QVariantMap &settings)
{
    if (m_serviceSettings == settings)
        return;

    m_serviceSettings = settings;
    connectToNode();

{% for property in interface.properties %}
{%   if property.type.is_model %}
    m_{{property}}->updateServiceSettings(settings);
{%   endif %}
{% endfor %}
}

{% if interface_zoned %}
void {{class}}::syncZones()
{
    if (m_replica.isNull())
        return;
    QRemoteObjectPendingReply<QStringList> zoneReply = m_replica->availableZones();
    auto zoneWatcher = new QRemoteObjectPendingCallWatcher(zoneReply, this);
    connect(zoneWatcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, zoneReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
            if (!m_synced) {
                m_zones = zoneReply.returnValue();
                for (const QString& zone : std::as_const(m_zones)) {
                    if (m_zoneMap.contains(zone))
                        continue;
                    auto zoneObject = new {{zone_class}}(zone, this);
                    m_zoneMap.insert(zone, zoneObject);
                    connect(zoneObject, &{{zone_class}}::syncDone, this, &{{class}}::onZoneSyncDone);
                }
                Q_EMIT availableZonesChanged(m_zones);

                for (const QString& zone : m_zoneMap.keys())
                    m_zoneMap.value(zone)->sync();
            } else {
                onZoneSyncDone();
            }
        }
        self->deleteLater();
    });
}

QStringList {{class}}::availableZones() const
{
    return m_zones;
}
{% endif %}

{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
{%     if not property.is_model %}
{{qtif.prop_setter(property, class, zoned=interface_zoned)}}
{
    if (m_replica.isNull())
        return;
{%     if not property.type.is_model %}
{%     if interface_zoned %}
    m_replica->set{{property|upperfirst}}({{property}}, zone);
{%     else %}
    m_replica->push{{property|upperfirst}}({{property}});
{%     endif %}
{%     else %}
    qCritical() << "{{class}}::{{property}}, remote models not supported";
{%     endif %}
}

{%     endif %}
{%   endif %}
{% endfor %}

{% for operation in interface.operations %}
{{ qtif.operation(operation, class, zoned=interface_zoned) }}
{
    if (m_replica.isNull())
        return QIfPendingReply<{{operation|return_type}}>::createFailedReply();
    else if (static_cast<QRemoteObjectReplica*>(m_replica.get())->state() != QRemoteObjectReplica::Valid)
        return QIfPendingReply<{{operation|return_type}}>::createFailedReply();

{% set function_parameters = operation.parameters|join(', ') %}
{% if interface_zoned %}
{%   if operation.parameters|length %}
{%     set function_parameters = function_parameters + ', ' %}
{%   endif %}
{%   set function_parameters = function_parameters + 'zone' %}
{% endif%}
    qCDebug(qLcRO{{interface}}) << "{{operation}} called";
    QRemoteObjectPendingReply<QVariant> reply = m_replica->{{operation}}({{function_parameters}});
    auto ifReply = m_helper->toQIfPendingReply<{{operation|return_type}}>(reply);

    //Pass an empty std::function to only handle errors.
    ifReply.then(std::function<void({{operation|return_type}})>(), [this]() {
        Q_EMIT errorChanged(QIfAbstractFeature::InvalidOperation, u"{{class}}, remote call of method {{operation}} failed"_s);
    });
    return ifReply;
}

{% endfor %}

bool {{class}}::connectToNode()
{
    auto findConnectionUrl = [this](const QString &key) -> QUrl {
        const auto it = m_serviceSettings.constFind(key);

        if (it != m_serviceSettings.constEnd())
            return it->toMap().value(u"connectionUrl"_s).toUrl();
        return QUrl();
    };

    auto findConnectionTimeout = [this](const QString &key, int defaultTimeout) -> int {
        const auto it = m_serviceSettings.constFind(key);

        if (it != m_serviceSettings.constEnd())
            return it->toMap().value(u"connectionTimeout"_s, defaultTimeout).toInt();
        return defaultTimeout;
    };

    QUrl url = findConnectionUrl(u"{{interface.qualified_name}}"_s);
    if (url.isEmpty())
        url = findConnectionUrl(u"{{interface}}"_s);
    if (url.isEmpty())
        url = findConnectionUrl(u"{{module}}"_s);
    if (url.isEmpty())
        url = m_serviceSettings.value(u"connectionUrl"_s).toUrl();

    static QString configPath;
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(u"{{module.module_name|lower}}"_s);
        url = QUrl(settings.value(u"Registry"_s).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcRO{{interface}}) << "SERVER_CONF_PATH environment variable is set.\n"
                                       << "Overriding service setting: '{{interface}}.connectionUrl'";

            qCInfo(qLcRO{{interface}}) << "Using SERVER_CONF_PATH is deprecated and will be removed"
                                          "in future Qt versions.";
            qCInfo(qLcRO{{interface}}) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty() && QFile::exists(u"./server.conf"_s)) {
        configPath = u"./server.conf"_s;

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(u"{{module.module_name|lower}}"_s);
        url = QUrl(settings.value(u"Registry"_s).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcRO{{interface}}) << "Reading url from ./server.conf.\n"
                                       << "Overriding service setting: '{{interface}}.connectionUrl'";

            qCInfo(qLcRO{{interface}}) << "Using ./server.conf. is deprecated and will be removed"
                                          "in future Qt versions.";
            qCInfo(qLcRO{{interface}}) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty())
        url = QIfRemoteObjectsHelper::buildDefaultUrl(u"{{module.module_name|lower}}"_s);

    if (m_url != url) {
        // QtRO doesn't allow to change the URL without destroying the Node
        if (m_node) {
            qCInfo(qLcRO{{interface}}) << "Disconnecting from" << m_url;
            delete m_node;
        }

        m_url = url;

        m_node = new QRemoteObjectNode();
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcRO{{interface}}) << "Connection to" << m_url << "failed!";
            m_replica.reset();
{% if interface_zoned %}
            m_synced = false;
{% endif %}
            return false;
        }
        qCInfo(qLcRO{{interface}}) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<{{interface}}Replica>(m_remoteObjectsLookupName));
        setupConnections();

        const int defaultTimeout = 3000;
        int connectionTimeout = defaultTimeout;
        connectionTimeout = findConnectionTimeout(u"{{interface.qualified_name}}"_s, defaultTimeout);

        if (connectionTimeout == defaultTimeout)
            connectionTimeout = findConnectionTimeout(u"{{interface}}"_s, defaultTimeout);

        if (connectionTimeout == defaultTimeout)
            connectionTimeout = findConnectionTimeout(u"{{module}}"_s, defaultTimeout);

        if (connectionTimeout == defaultTimeout)
            connectionTimeout = m_serviceSettings.value(u"connectionTimeout"_s, defaultTimeout).toInt();

        if (connectionTimeout != -1) {
            QTimer::singleShot(connectionTimeout, this, [this](){
                if(!m_replica->isInitialized())
                    qCWarning(qLcRO{{interface}}) << "{{interface.qualified_name}} wasn't initialized within the timeout period. Please make sure the server is running.";
            });
        }
    }
    return true;
}

void {{class}}::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIfRemoteObjectsReplicaHelper::onNodeError);
    connect(m_helper, &QIfRemoteObjectsReplicaHelper::errorChanged, this, &QIfFeatureInterface::errorChanged);

    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIfRemoteObjectsReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &{{interface}}Replica::pendingResultAvailable, m_helper, &QIfRemoteObjectsReplicaHelper::onPendingResultAvailable);
{% if interface_zoned %}
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &{{class}}::syncZones);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, this, [this](QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState){
        Q_UNUSED(oldState)
        if (newState == QRemoteObjectReplica::Suspect)
            m_synced = false;
    });
{% else %}
    //As the Replica is now initialized, this will trigger an update of all properties (not just the changed ones)
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &{{class}}::initialize);
{% endif %}
{% for property in interface.properties if not property.type.is_model %}
{%   if interface_zoned %}
    connect(m_replica.data(), &{{interface}}Replica::{{property}}Changed, this, [this]({{property|parameter_type}}, const QString &zone) {
        auto zoneObject = m_zoneMap.value(zone);
        if (!zoneObject) {
            qCCritical(qLcRO{{interface}}) << "Backend got changed signal for a zone which doesn't exist. Ignoring it.";
            return;
        }
        zoneObject->{{property|setter_name}}({{property}});
    });
{%   else %}
    connect(m_replica.data(), &{{interface}}Replica::{{property}}Changed, this, &{{class}}::{{property}}Changed);
{%   endif %}
{% endfor %}
{% for signal in interface.signals %}
    connect(m_replica.data(), &{{interface}}Replica::{{signal}}, this, &{{class}}::{{signal}});
{% endfor %}
}

{% if interface_zoned %}
void {{class}}::onZoneSyncDone()
{
    const QStringList zones = m_zoneMap.keys();

    for (const QString& zone : zones) {
        if (m_zoneMap.value(zone)->isSyncing())
            return;
    }

    m_synced = true;

    for (const QString& zone : zones)
        m_zoneMap.value(zone)->emitCurrentState();
    Q_EMIT initializationDone();
}
{% endif %}

{{ module|end_namespace }}

#include "moc_{{class|lower}}.cpp"
