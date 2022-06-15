{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}

#include "tst_custom.h"

CustomTest::CustomTest()
    : QObject()
{
}

void CustomTest::initTestCase()
{
    QVERIFY(true);
{% set foo = module|custom_filter %}
    qInfo("Test successfully generated from 'custom template'");
}

QTEST_MAIN(CustomTest)
