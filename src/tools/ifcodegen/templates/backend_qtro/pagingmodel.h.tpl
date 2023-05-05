{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% set interface_zoned = interface.tags.config and interface.tags.config.zoned  %}
{% if interface_zoned %}
{% set class = 'Zoned{0}RoModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}RoModelBackend'.format(property|upperfirst) %}
{% endif %}

#include <QIfPagingModelInterface>
#include "{{property.type.nested|lower}}.h"

#include "QtIfRemoteObjectsHelper/rep_qifpagingmodel_replica.h"

 using namespace Qt::StringLiterals;

{{ module|begin_namespace }}

class {{class}} : public QIfPagingModelInterface
{
    Q_OBJECT
public:
    explicit {{class}}(const QString &remoteObjectsLookupName = u"{{interface.qualified_name}}.{{property}}"_s, QObject *parent = nullptr);
    ~{{class}}() override;

    void initialize() override;
    void registerInstance(const QUuid &identifier) override;
    void unregisterInstance(const QUuid &identifier) override;

    void fetchData(const QUuid &identifier, int start, int count) override;

private:
    bool connectToNode();
    void setupConnections();

    QSharedPointer<QIfPagingModelReplica> m_replica;
    QIfRemoteObjectsReplicaHelper *m_helper;
    QRemoteObjectNode *m_node= nullptr;
    QString m_remoteObjectsLookupName;
    QUrl m_url;
    QVariantList m_list;
};

{{ module|end_namespace }}

