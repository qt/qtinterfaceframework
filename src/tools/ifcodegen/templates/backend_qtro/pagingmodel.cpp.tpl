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

Q_LOGGING_CATEGORY(qLcRO{{interface}}{{property|upper_first}}, "{{module|qml_type|lower}}.{{interface|lower}}backend.{{property|lower}}.remoteobjects", QtInfoMsg)

{{class}}::{{class}}(const QString &remoteObjectsLookupName, QObject* parent)
    : QIfPagingModelInterface(parent)
    , m_helper(new QIfRemoteObjectsReplicaHelper(qLcRO{{interface}}{{property|upper_first}}(), this))
    , m_node(nullptr)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
{
    qRegisterMetaType<QIfPagingModelInterface*>();
}

/*! \internal */
{{class}}::~{{class}}()
{
    delete m_node;
}

void {{class}}::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized())
        Q_EMIT initializationDone();
}

void {{class}}::registerInstance(const QUuid &identifier)
{
    if (m_replica.isNull())
        return;
    m_replica->registerInstance(identifier);
}

void {{class}}::unregisterInstance(const QUuid &identifier)
{
    if (m_replica.isNull())
        return;
    m_replica->unregisterInstance(identifier);
}

void {{class}}::fetchData(const QUuid &identifier, int start, int count)
{
    if (m_replica.isNull())
        return;
    m_replica->fetchData(identifier, start, count);
}

bool {{class}}::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("{{module.module_name|lower}}"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QIfRemoteObjectsHelper::buildDefaultUrl(QStringLiteral("{{module.module_name|lower}}"))).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode();
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcRO{{interface}}{{property|upper_first}}) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIfPagingModelReplica>(m_remoteObjectsLookupName));
        setupConnections();
    }
    return true;
}

void {{class}}::setupConnections()
{
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIfFeatureInterface::initializationDone);
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIfRemoteObjectsReplicaHelper::onNodeError);
    connect(m_helper, &QIfRemoteObjectsReplicaHelper::errorChanged, this, &QIfFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIfRemoteObjectsReplicaHelper::onReplicaStateChanged);

    connect(m_replica.data(), &QIfPagingModelReplica::supportedCapabilitiesChanged, this, &{{class}}::supportedCapabilitiesChanged);
    connect(m_replica.data(), &QIfPagingModelReplica::countChanged, this, &{{class}}::countChanged);
    connect(m_replica.data(), &QIfPagingModelReplica::dataFetched, this, &{{class}}::dataFetched);
    connect(m_replica.data(), &QIfPagingModelReplica::dataChanged, this, &{{class}}::dataChanged);
}
