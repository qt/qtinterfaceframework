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
{% set class = "Core" %}

#ifndef {{class|upper}}_H
#define {{class|upper}}_H

#include <QRemoteObjectNode>

QT_BEGIN_NAMESPACE

class {{class}} : public QObject
{
    Q_OBJECT
private:
    {{class}}(QObject *parent=nullptr);

public:
    virtual ~{{class}}();
    void init();
    static {{class}}* instance();
    QRemoteObjectRegistryHost* host() const;

private:
    void reportError(QRemoteObjectNode::ErrorCode code);

    static {{class}}* s_instance;
    QRemoteObjectRegistryHost* m_host;
};

QT_END_NAMESPACE

#endif
