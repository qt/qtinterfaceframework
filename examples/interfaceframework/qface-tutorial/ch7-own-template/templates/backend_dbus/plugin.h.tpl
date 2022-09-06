{#
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
#}
{% include "common/generated_comment.cpp.tpl" %}
{% set class = '{0}DBusPlugin'.format(module.module_name) %}
{% set oncedefine = '{0}_H_'.format(class|upper) %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QObject>
#include <QVector>
#include <QIfServiceInterface>

class {{class}} : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QIfServiceInterface_iid FILE "{{module.module_name|lower}}.json")
    Q_INTERFACES(QIfServiceInterface)

public:
    explicit {{class}}(QObject *parent = nullptr);

    QStringList interfaces() const override;
    QIfFeatureInterface* interfaceInstance(const QString &interface) const override;

private:
    QVector<QIfFeatureInterface *> m_interfaces;
};

#endif // {{oncedefine}}

