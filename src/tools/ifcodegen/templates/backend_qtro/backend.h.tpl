{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:COMM$
##
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## $QT_END_LICENSE$
##
##
##
##
##
##
##
##
#############################################################################
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

QT_BEGIN_NAMESPACE

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
    explicit {{class}}(const QString &remoteObjectsLookupName = QStringLiteral("{{interface.qualified_name}}"),
                       QObject *parent = nullptr);
    ~{{class}}();

    void initialize() override;

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
{% for property in interface.properties %}
{%   if property.type.is_model %}
    QIfPagingModelInterface *m_{{property}};
{%   endif %}
{% endfor %}
{% if interface_zoned %}
    bool m_synced;
    QHash<QString, {{zone_class}}*> m_zoneMap;
    QStringList m_zones;

    friend class {{zone_class}};
{% endif %}
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
