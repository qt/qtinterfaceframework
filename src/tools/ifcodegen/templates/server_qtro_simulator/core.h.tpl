{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = "Core" %}

#ifndef {{class|upper}}_H
#define {{class|upper}}_H

#include <QtDeprecationMarkers>
#if QT_DEPRECATED_SINCE(6, 7)

#include <QRemoteObjectNode>

{{ module|begin_namespace }}

class QT_DEPRECATED_VERSION_X_6_7("Please use the QIfRemoteObjectConfig class instead.")
{{class}} : public QObject
{
    Q_OBJECT
private:
    {{class}}(QObject *parent=nullptr);

public:
    ~{{class}}() override;
    void init();
    static {{class}}* instance();
    QRemoteObjectRegistryHost* host() const;

private:
    void reportError(QRemoteObjectNode::ErrorCode code);

    static {{class}}* s_instance;
    QRemoteObjectRegistryHost* m_host;
};

{{ module|end_namespace }}

#endif // QT_DEPRECATED_SINCE(6, 7)

#endif
