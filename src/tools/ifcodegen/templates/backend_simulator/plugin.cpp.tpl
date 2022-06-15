{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}SimulatorPlugin'.format(module.module_name|upperfirst) %}

#include "{{class|lower}}.h"

{% for interface in module.interfaces %}
#include "{{interface|lower}}backend.h"
{% endfor %}

#include <QStringList>
#include <QtInterfaceFramework/QIfSimulationEngine>

{{ module|begin_namespace }}

{% if module.tags.config.interfaceBuilder %}
extern {{class}}::InterfaceBuilder {{module.tags.config.interfaceBuilder}};
{% endif %}

/*!
   \class {{class}}
   \inmodule {{module}}

*/
/*! \internal */
{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
    , m_simulationEngine(new QIfSimulationEngine(QStringLiteral("{{module.name|lower}}"), this))
{
{% if module.tags.config.interfaceBuilder %}
    m_interfaces = {{module.tags.config.interfaceBuilder}}(this);
    Q_ASSERT(m_interfaces.size() == interfaces().size());
{% else %}
{%   for interface in module.interfaces %}
    auto {{interface}}Instance = new {{interface}}Backend(m_simulationEngine, this);
    //Register the types for the SimulationEngine
    {{module.module_name|upperfirst}}::registerQmlTypes(QStringLiteral("{{module|qml_type}}.simulation"), {{module.majorVersion}}, {{module.minorVersion}});
    m_simulationEngine->registerSimulationInstance({{interface}}Instance, "{{module|qml_type}}.simulation", {{module.majorVersion}}, {{module.minorVersion}}, "{{interface}}Backend");
    m_interfaces << {{interface}}Instance;
{%   endfor %}
{% if module.tags.config_simulator and module.tags.config_simulator.simulationFile %}
{%   set simulationFile = module.tags.config_simulator.simulationFile %}
{% else %}
{%   set simulationFile = "qrc:///simulation/" + module.module_name|lower + '_simulation.qml' %}
{% endif %}
    m_simulationEngine->loadSimulationData(QStringLiteral(":/simulation/{{module.module_name|lower}}_simulation_data.json"));
    m_simulationEngine->loadSimulation(QUrl(QStringLiteral("{{simulationFile}}")));
{% endif %}
}

/*! \internal */
QStringList {{class}}::interfaces() const
{
    QStringList list;
{% for iface in module.interfaces %}
{%   if loop.first %}    list{% endif %} << {{module.module_name|upperfirst}}_{{iface}}_iid{% if loop.last %};{% endif %}
{% endfor %}

    return list;
}

/*! \internal */
QIfFeatureInterface *{{class}}::interfaceInstance(const QString &interface) const
{
     int index = interfaces().indexOf(interface);
     return index < 0 ? nullptr : m_interfaces.at(index);
}

{{ module|end_namespace }}
