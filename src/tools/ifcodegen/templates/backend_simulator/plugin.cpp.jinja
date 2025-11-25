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

#include <QStringList>
#include <QtInterfaceFramework/QIfSimulationEngine>

using namespace Qt::StringLiterals;

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
    , m_simulationEngine(new QIfSimulationEngine(u"{{module.name|lower}}"_s, this))
{
#ifdef QT_STATICPLUGIN
    Q_INIT_RESOURCE({{module.module_name|lower}}_simulation);
#endif
{% if module.tags.config.interfaceBuilder %}
    QVector<QIfFeatureInterface *> interfacesList = {{module.tags.config.interfaceBuilder}}(this);
    Q_ASSERT(interfacesList.size() == interfaces().size());
{%   for interface in module.interfaces %}
    m_{{interface|lower}}Backend = static_cast<{{interface}}Backend*>(interfacesList.at({{loop.index0}}));
{%   endfor %}
{% else %}
    //Register the types for the SimulationEngine
    {{module.module_name|upperfirst}}::registerQmlTypes(u"{{module|qml_type}}.simulation"_s, {{module.majorVersion}}, {{module.minorVersion}});
{%   for interface in module.interfaces %}
    m_{{interface|lower}}Backend = new {{interface}}Backend(m_simulationEngine, this);
    m_simulationEngine->registerSimulationInstance(m_{{interface|lower}}Backend, "{{module|qml_type}}.simulation", {{module.majorVersion}}, {{module.minorVersion}}, "{{interface}}Backend");
{%   endfor %}
{% if module.tags.config_simulator and module.tags.config_simulator.simulationFile %}
{%   set simulationFile = module.tags.config_simulator.simulationFile %}
{% else %}
{%   set simulationFile = "qrc:///simulation/" + module.module_name|lower + '_simulation.qml' %}
{% endif %}
    m_simulationEngine->loadSimulationData(u":/simulation/{{module.module_name|lower}}_simulation_data.json"_s);
    m_simulationEngine->loadSimulation(QUrl(u"{{simulationFile}}"_s));
{% endif %}
}

/*! \internal */
QStringList {{class}}::interfaces() const
{
    QStringList list;
{% for iface in module.interfaces %}
{%   if loop.first %}    list{% endif %} << QStringLiteral({{module.module_name|upperfirst}}_{{iface}}_iid){% if loop.last %};{% endif %}
{% endfor %}

    return list;
}

/*! \internal */
QIfFeatureInterface *{{class}}::interfaceInstance(const QString &interface) const
{
{% for interface in module.interfaces %}
{%   if loop.first %}
    if (interface == QStringLiteral({{module.module_name|upperfirst}}_{{interface}}_iid))
{%   else %}
    else if (interface == QStringLiteral({{module.module_name|upperfirst}}_{{interface}}_iid))
{%   endif %}
        return m_{{interface|lower}}Backend;
{% endfor %}

    return nullptr;
}

QString {{class}}::id() const
{
{% if module.tags.config_simulator and module.tags.config_simulator.serviceObjectId %}
{%   set serviceObjectId = module.tags.config_simulator.serviceObjectId %}
{% else %}
{%   set serviceObjectId = "{0}_simulation".format(module.name) %}
{% endif %}
    return u"{{serviceObjectId}}"_s;
}

QString {{class}}::configurationId() const
{
{% if module.tags.config_simulator and module.tags.config_simulator.configurationId %}
{%   set configurationId = module.tags.config_simulator.configurationId %}
{% elif module.tags.config.configurationId %}
{%   set configurationId = module.tags.config.configurationId %}
{% else %}
{%   set configurationId = module.name %}
{% endif %}
    return u"{{configurationId}}"_s;
}

void {{class}}::updateServiceSettings(const QVariantMap &settings)
{
{% for interface in module.interfaces %}
    m_{{interface|lower}}Backend->updateServiceSettings(settings);
{% endfor %}
}

{{ module|end_namespace }}

#include "moc_{{class|lower}}.cpp"
