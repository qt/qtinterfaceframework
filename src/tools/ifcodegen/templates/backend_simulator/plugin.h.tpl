{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}SimulatorPlugin'.format(module.module_name|upperfirst) %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QtInterfaceFramework/QIfServiceInterface>

{% for interface in module.interfaces %}
#include "{{interface|lower}}backend.h"
{% endfor %}

QT_FORWARD_DECLARE_CLASS(QIfSimulationEngine)

{{ module|begin_namespace }}

class {{class}} : public QObject, QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "{{module.module_name|lower}}.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    typedef QVector<QIfFeatureInterface *> (InterfaceBuilder)({{class}} *);

    explicit {{class}}(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface* interfaceInstance(const QString& interface) const override;

    QString id() const override;
    QString configurationId() const override;
    void updateServiceSettings(const QVariantMap &settings) override;

private:
{% for interface in module.interfaces %}
    {{interface}}Backend *m_{{interface|lower}}Backend;
{% endfor %}
    QIfSimulationEngine *m_simulationEngine;
};

{{ module|end_namespace }}

#endif // {{oncedefine}}
