{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}ModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}ModelBackend'.format(property|upperfirst) %}
{% endif %}

{{ module|begin_namespace }}

{{class}}::{{class}}(QObject* parent)
    : QIfPagingModelInterface(parent)
{
    qRegisterMetaType<QIfPagingModelInterface*>();
}

/*! \internal */
{{class}}::~{{class}}()
{
}

int {{class}}::count() const
{
    return m_list.count();
}

void {{class}}::initialize()
{
    QIF_SIMULATION_TRY_CALL({{class}}, "initialize", void);

    Q_EMIT initializationDone();
}

void {{class}}::registerInstance(const QUuid &identifier)
{
    QIF_SIMULATION_TRY_CALL({{class}}, "registerInstance", void, identifier);

    Q_EMIT countChanged(identifier, m_list.count());
}

void {{class}}::unregisterInstance(const QUuid &identifier)
{
    QIF_SIMULATION_TRY_CALL({{class}}, "unregisterInstance", void, identifier);
}

void {{class}}::fetchData(const QUuid &identifier, int start, int count)
{
    QIF_SIMULATION_TRY_CALL({{class}}, "fetchData", void, identifier, start, count);

    QVariantList list;
    int max = qMin(start + count, m_list.count());
    for(int i=start; i < max; i++)
        list.append(QVariant::fromValue(m_list.at(i)));

    Q_EMIT dataFetched(identifier, list, start, max <  m_list.count());
}

void {{class}}::insert(int index, const {{property.type.nested}} &item)
{
    m_list.insert(index, item);

    Q_EMIT dataChanged(QUuid(), { QVariant::fromValue(item) }, index, 0);
    Q_EMIT countChanged(QUuid(), m_list.count());
}

void {{class}}::remove(int index)
{
    m_list.removeAt(index);

    Q_EMIT dataChanged(QUuid(), QVariantList(), index, 1);
    Q_EMIT countChanged(QUuid(), m_list.count());
}

void {{class}}::move(int currentIndex, int newIndex)
{
    int min = qMin(currentIndex, newIndex);
    int max = qMax(currentIndex, newIndex);

    m_list.move(currentIndex, newIndex);
    QVariantList variantList;
    for (int i = min; i <= max; i++)
        variantList.append(QVariant::fromValue(m_list.at(i)));

    Q_EMIT dataChanged(QUuid(), variantList, min, max - min + 1);
}

void {{class}}::reset()
{
    Q_EMIT dataChanged(QUuid(), QVariantList(), 0, m_list.count());
    Q_EMIT countChanged(QUuid(), 0);
    m_list.clear();
}

void {{class}}::update(int index, const {{property.type.nested}} &item)
{
    m_list[index] = item;
    Q_EMIT dataChanged(QUuid(), { QVariant::fromValue(item) }, index, 1);
}

{{property.type.nested}} {{class}}::at(int index) const
{
    return m_list.at(index);
}

{{ module|end_namespace }}
