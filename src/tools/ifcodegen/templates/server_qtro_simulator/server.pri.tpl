{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# SPDX-License-Identifier: LicenseRef-Qt-Commercial
#}
{% include "common/generated_comment.qmake.tpl" %}

QT += gui remoteobjects ifremoteobjects_helper ifremoteobjects_helper_private

HEADERS += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}backend.h \
    $$PWD/{{interface|lower}}adapter.h \
{% endfor %}
    $$PWD/core.h

SOURCES += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}backend.cpp \
    $$PWD/{{interface|lower}}adapter.cpp \
{% endfor %}
    $$PWD/core.cpp \
    $$PWD/main.cpp \

REPC_SOURCE += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}.rep \
{% endfor %}

RESOURCES += $$PWD/{{module.module_name|lower}}_simulation.qrc

OTHER_FILES += \
    $$PWD/{{module.module_name|lower}}.json \
    $$PWD/{{module.module_name|lower}}_simulation_data.json
