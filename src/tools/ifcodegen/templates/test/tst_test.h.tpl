{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
#include <QtTest>

QT_FORWARD_DECLARE_CLASS(QIfServiceManager);

{{ module|begin_namespace }}

class {{interface}}Test : public QObject
{
    Q_OBJECT

public:
    {{interface}}Test();

public Q_SLOTS:
    void init();

    void testInitBackend();
    void testWithoutBackend();
    void testInvalidBackend();
    void testClearServiceObject();
    void testChangeFromBackend();
    void testChangeFromFrontend();
    void testMethods();
    void testSignals();
    void testModels();

private:
    QIfServiceManager *manager;
};

{{ module|end_namespace }}
