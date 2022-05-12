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
{% include "common/generated_comment.cpp.tpl" %}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}ModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}ModelBackend'.format(property|upperfirst) %}
{% endif %}

#include <QIfPagingModelInterface>
#include "{{property.type.nested|lower}}.h"

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

