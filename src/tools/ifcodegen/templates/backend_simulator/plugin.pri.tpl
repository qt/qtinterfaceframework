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
{% include "common/generated_comment.qmake.tpl" %}

HEADERS += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}backend.h \
{% endfor %}
    $$PWD/{{module.module_name|lower}}simulatorplugin.h

SOURCES += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}backend.cpp \
{% endfor %}
    $$PWD/{{module.module_name|lower}}simulatorplugin.cpp

RESOURCES += $$PWD/{{module.module_name|lower}}_simulation.qrc

OTHER_FILES += \
    $$PWD/{{module.module_name|lower}}.json \
    $$PWD/{{module.module_name|lower}}_simulation_data.json
