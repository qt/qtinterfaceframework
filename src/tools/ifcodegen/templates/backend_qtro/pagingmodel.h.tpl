{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
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
{% set class = 'Zoned{0}RoModelBackend'.format(property|upperfirst) %}
{% else %}
{% set class = '{0}RoModelBackend'.format(property|upperfirst) %}
{% endif %}

#include <QIfPagingModelInterface>
#include "{{property.type.nested|lower}}.h"

#include "QtIfRemoteObjectsHelper/rep_qifpagingmodel_replica.h"

class {{class}} : public QIfPagingModelInterface
{
    Q_OBJECT
public:
    explicit {{class}}(const QString &remoteObjectsLookupName = QStringLiteral("{{interface.qualified_name}}.{{property}}"), QObject *parent = nullptr);
    ~{{class}}();

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

