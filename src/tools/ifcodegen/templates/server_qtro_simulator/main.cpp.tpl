{#
#############################################################################
##
## Copyright (C) 2021 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################
#}
{% include "common/generated_comment.cpp.tpl" %}
#include <QGuiApplication>
#include <QCommandLineParser>
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
{% if module.tags.config_simulator and module.tags.config_simulator.defaultServerMode %}
{%   set defaultMode = module.tags.config_simulator.defaultServerMode %}
{% else %}
{%   set defaultMode = "headless" %}
{% endif %}
{% if defaultMode == "headless" %}
    bool gui = false;
{% elif defaultMode == "gui" %}
    bool gui = true;
{% else %}
{{   error("Unknown value in 'config_simulator.defaultServerMode'. Valid modes are: 'headless', 'gui'")}}
{% endif %}
    bool guiOptionSet = false;
    bool headlessOptionSet = false;
    for (int i=1; i<argc; i++) {
        if (qstrcmp(argv[i], "--gui") == 0) {
            gui = true;
            guiOptionSet = true;
        } else if (qstrcmp(argv[i], "--headless") == 0) {
            gui = false;
            headlessOptionSet = true;
        }
    }

    if (guiOptionSet && headlessOptionSet)
        qFatal("--gui and --headless can't be used at the same time!");

    QScopedPointer<QCoreApplication> app;
    if (gui)
        app.reset(new QGuiApplication(argc, argv));
    else
        app.reset(new QCoreApplication(argc, argv));

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption guiOption("gui", "Gui mode. Starts using a QGuiApplication and allows "
                                        "instantiating visual elements in the simulation code");
    parser.addOption(guiOption);
    QCommandLineOption headlessOption("headless", "Headless mode. Starts using a QCoreApplication "
                                                  "and does NOT allow instantiating visual elements "
                                                  "in the simulation code");
    parser.addOption(headlessOption);

    parser.process(qApp->arguments());

    // single instance guard
    QLockFile lockFile(QStringLiteral("%1/%2.lock").arg(QDir::tempPath(), qApp->applicationName()));
    if (!lockFile.tryLock(100)) {
        qCritical("%s already running, aborting...", qPrintable(qApp->applicationName()));
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


    return qApp->exec();
}
