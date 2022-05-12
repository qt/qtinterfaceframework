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
{% include "common/generated_comment.cpp.tpl" %}
#include <QtTest>
{% for interface in module.interfaces %}
#include "tst_{{interface|lower}}.h"
{% endfor %}

int main(int argc, char *argv[])
{
    int returnCode = 0;
    {% for interface in module.interfaces %}
    {{interface}}Test {{interface|lower}};
    returnCode += QTest::qExec(&{{interface|lower}}, argc, argv);
    {% endfor %}

    return returnCode;
}
