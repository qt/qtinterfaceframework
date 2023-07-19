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
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}
#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QRemoteObjectNode>
#include <QtIfRemoteObjectsHelper/QIfRemoteObjectsReplicaHelper>
{% if module.tags.config.module %}
#include <{{module.tags.config.module}}/{{interface}}BackendInterface>
{% else %}
#include "{{interface|lower}}backendinterface.h"
{% endif %}
#include "rep_{{interface|lower}}_replica.h"

{% for property in interface.properties %}
{%   if property.type.is_model %}
{% include "pagingmodel.h.tpl" %}
{%   endif %}
{% endfor %}

using namespace Qt::StringLiterals;

{{ module|begin_namespace }}

{% if interface_zoned %}
class {{class}};

class {{zone_class}} : public QObject
{
    Q_OBJECT

public:
    explicit {{zone_class}}(const QString &zone, {{class}} *parent = nullptr);

    bool isSyncing();
    void sync();

public Q_SLOTS:
{% for property in interface.properties %}
    {{qtif.prop_setter(property, model_interface = true)}};
{% endfor %}
    void emitCurrentState();

Q_SIGNALS:
    void syncDone();

private:
    void checkSync();

    {{class}} *m_parent;
    QString m_zone;
{% for property in interface.properties %}
{%   if property.type.is_model %}
    QIfPagingModelInterface *m_{{ property }};
{%   else %}
    {{ property|return_type }} m_{{ property }};
{%   endif %}
{% endfor %}
    QStringList m_propertiesToSync;
};
{% endif %}

class {{class}} : public {{interface}}BackendInterface
{
    Q_OBJECT

public:
    explicit {{class}}(const QString &remoteObjectsLookupName = u"{{interface.qualified_name}}"_s,
                       QObject *parent = nullptr);
    ~{{class}}() override;

    void initialize() override;
    void updateServiceSettings(const QVariantMap &settings);

public Q_SLOTS:
{% for property in interface.properties %}
{%   if not property.readonly and not property.const %}
{%     if not property.is_model %}
    {{qtif.prop_setter(property, zoned=interface_zoned)}} override;
{%     endif %}
{%   endif %}
{% endfor %}

{% if interface_zoned %}
    QStringList availableZones() const override;
{% endif %}

{% for operation in interface.operations %}
    {{qtif.operation(operation, zoned=interface_zoned)}} override;
{% endfor %}

protected Q_SLOTS:
{% if interface_zoned %}
    void syncZones();
    void onZoneSyncDone();
{% endif %}

protected:
    bool connectToNode();
    void setupConnections();

    QSharedPointer<{{interface}}Replica> m_replica;
    QRemoteObjectNode* m_node= nullptr;
    QUrl m_url;
    QString m_remoteObjectsLookupName;
    QHash<quint64, QIfPendingReplyBase> m_pendingReplies;
    QIfRemoteObjectsReplicaHelper *m_helper;
    QVariantMap m_serviceSettings;
{% for property in interface.properties %}
{%   if property.type.is_model %}
{%     if interface_zoned %}
    Zoned{{property|upperfirst}}RoModelBackend *m_{{property}};
{%     else %}
    {{property|upperfirst}}RoModelBackend *m_{{property}};
{%     endif %}
{%   endif %}
{% endfor %}
{% if interface_zoned %}
    bool m_synced;
    QHash<QString, {{zone_class}}*> m_zoneMap;
    QStringList m_zones;

    friend class {{zone_class}};
{% endif %}
};

{{ module|end_namespace }}

#endif // {{oncedefine}}
