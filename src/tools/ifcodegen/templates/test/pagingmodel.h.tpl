{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Copyright (C) 2018 Pelagicore AG
## Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}Model'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}Model'.format(property|upperfirst) %}
{% endif %}

#include "{{property.type.nested|lower}}.h"
#include <QtDebug>

#include <QIfPagingModelInterface>

class {{class}} : public QIfPagingModelInterface
{
    Q_OBJECT
public:
    explicit {{class}}(QObject *parent = nullptr)
        : QIfPagingModelInterface(parent)
    {
        for(int i=0; i < 100; i++)
            m_list.append(QVariant::fromValue({{property.type.nested|test_type_value}}));
    }

    ~{{class}}()
    {
    }

    void initialize() override
    {
        Q_EMIT initializationDone();
    }

    void registerInstance(const QUuid &identifier) override
    {
        Q_EMIT countChanged(identifier, 100);
    }

    void unregisterInstance(const QUuid &identifier) override
    {
        Q_UNUSED(identifier);
    }

    void fetchData(const QUuid &identifier, int start, int count) override
    {
        QVariantList list;
        int max = qMin(start + count, m_list.count());
        for(int i=start; i < max; i++)
            list.append(m_list.at(i));

        Q_EMIT dataFetched(identifier, list, start, max <  m_list.count());
    }

private:
    QVariantList m_list;
};
