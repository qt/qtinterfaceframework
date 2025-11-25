{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.qmake.tpl" %}

# Export all symbols when building this library
DEFINES *= QT_BUILD_{{module|upper|replace('.', '_')|strip_QT}}_LIB

{% if not module.tags.config.disablePrivateIF %}
QT *= interfaceframework-private
{% endif %}

HEADERS += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}.h \
    $$PWD/{{interface|lower}}_p.h \
    $$PWD/{{interface|lower}}backendinterface.h \
{% endfor %}
{% for struct in module.structs %}
    $$PWD/{{struct|lower}}.h \
{% endfor %}
    $$PWD/{{module.module_name|lower}}.h \
    $$PWD/{{module.module_name|lower}}factory.h \
    $$PWD/{{module.module_name|lower}}global.h

SOURCES += \
{% for interface in module.interfaces %}
    $$PWD/{{interface|lower}}.cpp \
    $$PWD/{{interface|lower}}backendinterface.cpp \
{% endfor %}
{% for struct in module.structs %}
    $$PWD/{{struct|lower}}.cpp \
{% endfor %}
    $$PWD/{{module.module_name|lower}}.cpp \
    $$PWD/{{module.module_name|lower}}factory.cpp
