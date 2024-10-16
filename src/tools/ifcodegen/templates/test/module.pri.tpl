{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.qmake.tpl" %}

HEADERS += \
{% for interface in module.interfaces %}
    $$PWD/tst_{{interface|lower}}.h \
{% endfor %}

SOURCES += \
{% for interface in module.interfaces %}
    $$PWD/tst_{{interface|lower}}.cpp \
{% endfor %}
    $$PWD/main.cpp \
