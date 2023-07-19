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

using namespace Qt::StringLiterals;

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

void {{class}}::updateServiceSettings(const QVariantMap &settings)
{
    if (m_serviceSettings == settings)
        return;

    m_serviceSettings = settings;
    connectToNode();
}

bool {{class}}::connectToNode()
{
    QUrl url;
    const auto it = m_serviceSettings.constFind(u"{{interface}}"_s);

    if (it != m_serviceSettings.constEnd())
        url = it->toMap().value(u"connectionUrl"_s).toUrl();
    if (url.isEmpty())
        url = m_serviceSettings.value(u"connectionUrl"_s).toUrl();

    static QString configPath;
    if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
        configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(u"{{module.module_name|lower}}"_s);
        url = QUrl(settings.value(u"Registry"_s).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "SERVER_CONF_PATH environment variable is set.\n"
                                       << "Overriding service setting: '{{interface}}.connectionUrl'";

            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Using SERVER_CONF_PATH is deprecated and will be removed"
                                          "in future Qt versions.";
            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty() && QFile::exists(u"./server.conf"_s)) {
        configPath = u"./server.conf"_s;

        QSettings settings(configPath, QSettings::IniFormat);
        settings.beginGroup(u"{{module.module_name|lower}}"_s);
        url = QUrl(settings.value(u"Registry"_s).toString());
        if (!url.isEmpty()) {
            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Reading url from ./server.conf.\n"
                                       << "Overriding service setting: '{{interface}}.connectionUrl'";

            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Using ./server.conf. is deprecated and will be removed"
                                          "in future Qt versions.";
            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Please use the new InterfaceFrameworkConfiguration item"
                                          " and set the backend specific configuration options (using"
                                          " the serviceSettings property).";
        }
    }

    if (url.isEmpty())
        url = QIfRemoteObjectsHelper::buildDefaultUrl(u"{{module.module_name|lower}}"_s);

    if (m_url != url) {
        // QtRO doesn't allow to change the URL without destroying the Node
        if (m_node) {
            qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Disconnecting from" << m_url;
            delete m_node;
        }

        m_url = url;

        m_node = new QRemoteObjectNode();
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcRO{{interface}}{{property|upper_first}}) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcRO{{interface}}{{property|upper_first}}) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIfPagingModelReplica>(m_remoteObjectsLookupName));
        setupConnections();

        const int defaultTimeout = 3000;
        int connectionTimeout = defaultTimeout;
        if (it != m_serviceSettings.constEnd())
            connectionTimeout = it->toMap().value(u"connectionTimeout"_s, defaultTimeout).toInt();

        if (connectionTimeout == defaultTimeout)
            connectionTimeout = m_serviceSettings.value(u"connectionTimeout"_s, defaultTimeout).toInt();

        if (connectionTimeout != -1) {
            QTimer::singleShot(connectionTimeout, this, [this](){
                if(!m_replica->isInitialized())
                    qCWarning(qLcRO{{interface}}{{property|upper_first}}) << "{{interface.qualified_name}}.{{property|upper_first}} wasn't initialized within the timeout period. Please make sure the server is running.";
            });
        }
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
