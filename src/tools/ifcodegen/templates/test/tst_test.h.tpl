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
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% set testModels = false %}
{% for property in interface.properties if not once %}
{%   if property.type.is_model %}
{%     set testModels = true %}
{%   endif %}
{% endfor %}
#include <QtTest>

QT_FORWARD_DECLARE_CLASS(QIfServiceManager);

class {{interface}}Test : public QObject
{
    Q_OBJECT

public:
    {{interface}}Test();

private Q_SLOTS:
    void init();

    void testInitBackend();
    void testWithoutBackend();
    void testInvalidBackend();
    void testClearServiceObject();
    void testChangeFromBackend();
    void testChangeFromFrontend();
    void testMethods();
    void testSignals();
{% if testModels %}
    void testModels();
{% endif %}

private:
    QIfServiceManager *manager;
};
