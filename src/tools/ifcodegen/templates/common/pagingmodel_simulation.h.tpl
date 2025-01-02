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

#include <QIfPagingModelInterface>
#include "{{property.type.nested|lower}}.h"

{{ module|begin_namespace }}

class {{class}} : public QIfPagingModelInterface
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit {{class}}(QObject *parent = nullptr);
    ~{{class}}();

    int count() const;

    Q_INVOKABLE void initialize() override;
    Q_INVOKABLE void registerInstance(const QUuid &identifier) override;
    Q_INVOKABLE void unregisterInstance(const QUuid &identifier) override;

    Q_INVOKABLE void fetchData(const QUuid &identifier, int start, int count) override;

public Q_SLOTS:
    void insert(int index, const {{property.type.nested}} &item);
    void remove(int index);
    void move(int currentIndex, int newIndex);
    void reset();
    void update(int index, const {{property.type.nested}} &item);
    {{property.type.nested}} at(int index) const;

private:
    QList<{{property.type.nested}}> m_list;
};

{{ module|end_namespace }}
