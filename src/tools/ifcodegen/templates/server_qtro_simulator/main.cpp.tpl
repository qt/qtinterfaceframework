{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
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
{% include "common/generated_comment.cpp.tpl" %}
#include <QCoreApplication>
#include <QDir>
#include <QLockFile>

{% for interface in module.interfaces %}
#include "{{interface|lower}}backend.h"
#include "{{interface|lower}}adapter.h"
{% endfor %}

#include "core.h"
#include <QtInterfaceFramework/QIfSimulationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // single instance guard
    QLockFile lockFile(QStringLiteral("%1/%2.lock").arg(QDir::tempPath(), app.applicationName()));
    if (!lockFile.tryLock(100)) {
        qCritical("%s already running, aborting...", qPrintable(app.applicationName()));
        return EXIT_FAILURE;
    }

    auto simulationEngine = new QIfSimulationEngine(QStringLiteral("{{module.name|lower}}"));

{% for interface in module.interfaces %}
    auto {{interface|lowerfirst}}Instance = new {{interface}}Backend(simulationEngine);
    //Register the types for the SimulationEngine
    {{module.module_name|upperfirst}}::registerQmlTypes(QStringLiteral("{{module|qml_type}}.simulation"), {{module.majorVersion}}, {{module.minorVersion}});
    simulationEngine->registerSimulationInstance({{interface|lowerfirst}}Instance, "{{module|qml_type}}.simulation", {{module.majorVersion}}, {{module.minorVersion}}, "{{interface}}Backend");
{% endfor %}
{% if module.tags.config_simulator and module.tags.config_simulator.simulationFile %}
{%   set simulationFile = module.tags.config_simulator.simulationFile %}
{% else %}
{%   set simulationFile = "qrc:///simulation/" + module.module_name|lower + '_simulation.qml' %}
{% endif %}
    simulationEngine->loadSimulationData(QStringLiteral(":/simulation/{{module.module_name|lower}}_simulation_data.json"));
    simulationEngine->loadSimulation(QUrl(QStringLiteral("{{simulationFile}}")));

    //initialize all our backends
{% for interface in module.interfaces %}
    {{interface|lowerfirst}}Instance->initialize();
{%   for property in interface.properties %}
{%     if property.type.is_model %}
    {{interface|lowerfirst}}Instance->{{property|getter_name}}()->initialize();
{%     endif %}
{%   endfor %}
{% endfor %}

    //Start Remoting the backends
{% for interface in module.interfaces %}
    auto {{interface|lowerfirst}}Adapter = new {{interface}}QtRoAdapter({{interface|lowerfirst}}Instance);
    {{interface|lowerfirst}}Adapter->enableRemoting(Core::instance()->host());
{% endfor %}


    return app.exec();
}
