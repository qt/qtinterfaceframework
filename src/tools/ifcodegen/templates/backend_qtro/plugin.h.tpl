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
{% set class = '{0}RoPlugin'.format(module.module_name) %}
{% set oncedefine = '{0}_{1}_H_'.format(module.module_name|upper, class|upper) %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QVector>
#include <QtInterfaceFramework/QIfServiceInterface>

QT_BEGIN_NAMESPACE

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

private:
    QVector<QIfFeatureInterface *> m_interfaces;
};

QT_END_NAMESPACE

#endif // {{oncedefine}}
