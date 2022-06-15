// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "filterandbrowsemodel.h"
#include "qifqmlconversion_helper.h"

#include <QTimer>
#include <QSettings>
#include <QIODevice>

Q_LOGGING_CATEGORY(qLcROQIfFilterAndBrowseModel, "interfaceframework.media.qifsearchandbrowsebackend.remoteobjects", QtInfoMsg)

QDataStream &operator<<(QDataStream &stream, const SearchAndBrowseItem &obj)
{
    stream << obj.name();
    stream << obj.type();
    stream << obj.url();
    stream << QVariant(obj.data());
    return stream;
}

QDataStream &operator>>(QDataStream &stream, SearchAndBrowseItem &obj)
{
    QString name;
    QString type;
    QUrl url;
    QVariant data;
    stream >> name;
    stream >> type;
    stream >> url;
    stream >> data;
    obj.setName(name);
    obj.setType(type);
    obj.setUrl(url);
    obj.setData(data.toMap());
    return stream;
}

FilterAndBrowseModel::FilterAndBrowseModel(QObject *parent, const QString& remoteObjectsLookupName)
    : QIfFilterAndBrowseModelInterface(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_node(nullptr)
    , m_helper(new QIfRemoteObjectsReplicaHelper(qLcROQIfFilterAndBrowseModel(), this))
{
    qRegisterMetaType<SearchAndBrowseItem>();
    qRegisterMetaType<QIfAudioTrackItem>();
}

void FilterAndBrowseModel::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized()) {
        emit availableContentTypesChanged(m_replica->availableContentTypes());
        emit initializationDone();
    }

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIfFilterAndBrowseModel) << "QIfFilterAndBrowseModel wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void FilterAndBrowseModel::registerInstance(const QUuid &identifier)
{
    m_replica->registerInstance(identifier);
}

void FilterAndBrowseModel::unregisterInstance(const QUuid &identifier)
{
    m_replica->unregisterInstance(identifier);
}

void FilterAndBrowseModel::fetchData(const QUuid &identifier, int start, int count)
{
    m_replica->fetchData(identifier, start, count);
}

void FilterAndBrowseModel::setContentType(const QUuid &identifier, const QString &contentType)
{
    m_replica->setContentType(identifier, contentType);
}

void FilterAndBrowseModel::setupFilter(const QUuid &identifier, QIfAbstractQueryTerm *term, const QList<QIfOrderTerm> &orderTerms)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    if (term)
        stream << term;
    m_replica->setupFilter(identifier, QVariant(data), orderTerms);
}

QIfPendingReply<QString> FilterAndBrowseModel::goBack(const QUuid &identifier)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->goBack(identifier);
    auto ifReply = m_helper->toQIfPendingReply<QString>(reply);

    //Pass an empty std::function to only handle errors.
    ifReply.then(std::function<void(QString)>(), [this]() {
        emit errorChanged(QIfAbstractFeature::InvalidOperation, QStringLiteral("remote call of method goBack failed"));
    });
    return ifReply;
}

QIfPendingReply<QString> FilterAndBrowseModel::goForward(const QUuid &identifier, int index)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->goForward(identifier, index);

    //Pass an empty std::function to only handle errors.
    auto ifReply = m_helper->toQIfPendingReply<QString>(reply);
    ifReply.then(std::function<void(QString)>(), [this]() {
        emit errorChanged(QIfAbstractFeature::InvalidOperation, QStringLiteral("remote call of method goForward failed"));
    });
    return ifReply;
}

QIfPendingReply<void> FilterAndBrowseModel::insert(const QUuid &identifier, int index, const QVariant &item)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->insert(identifier, index, item);

    //Pass an empty std::function to only handle errors.
    auto ifReply = m_helper->toQIfPendingReply<void>(reply);
    ifReply.then(std::function<void()>(), [this]() {
        emit errorChanged(QIfAbstractFeature::InvalidOperation, QStringLiteral("remote call of method insert failed"));
    });
    return ifReply;
}

QIfPendingReply<void> FilterAndBrowseModel::remove(const QUuid &identifier, int index)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->remove(identifier, index);

    //Pass an empty std::function to only handle errors.
    auto ifReply = m_helper->toQIfPendingReply<void>(reply);
    ifReply.then(std::function<void()>(), [this]() {
        emit errorChanged(QIfAbstractFeature::InvalidOperation, QStringLiteral("remote call of method remove failed"));
    });
    return ifReply;
}

QIfPendingReply<void> FilterAndBrowseModel::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->move(identifier, currentIndex, newIndex);

    //Pass an empty std::function to only handle errors.
    auto ifReply = m_helper->toQIfPendingReply<void>(reply);
    ifReply.then(std::function<void()>(), [this]() {
        emit errorChanged(QIfAbstractFeature::InvalidOperation, QStringLiteral("remote call of method move failed"));
    });
    return ifReply;
}

QIfPendingReply<int> FilterAndBrowseModel::indexOf(const QUuid &identifier, const QVariant &item)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->indexOf(identifier, item);

    //Pass an empty std::function to only handle errors.
    auto ifReply = m_helper->toQIfPendingReply<int>(reply);
    ifReply.then(std::function<void(int)>(), [this]() {
        emit errorChanged(QIfAbstractFeature::InvalidOperation, QStringLiteral("remote call of method indexOf failed"));
    });
    return ifReply;
}

bool FilterAndBrowseModel::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcROQIfFilterAndBrowseModel) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("qtifmedia"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:qtifmedia")).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode(this);
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIfFilterAndBrowseModel) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcROQIfFilterAndBrowseModel) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIfFilterAndBrowseModelReplica>(m_remoteObjectsLookupName));
        setupConnections();
    }
    return true;
}

void FilterAndBrowseModel::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIfRemoteObjectsReplicaHelper::onNodeError);
    connect(m_helper, &QIfRemoteObjectsReplicaHelper::errorChanged, this, &QIfFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIfRemoteObjectsReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIfFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::pendingResultAvailable, m_helper, &QIfRemoteObjectsReplicaHelper::onPendingResultAvailable);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::canGoBackChanged, this, &FilterAndBrowseModel::canGoBackChanged);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::canGoForwardChanged, this, &FilterAndBrowseModel::canGoForwardChanged);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::supportedCapabilitiesChanged, this, &FilterAndBrowseModel::supportedCapabilitiesChanged);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::queryIdentifiersChanged, this, &FilterAndBrowseModel::queryIdentifiersChanged);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::availableContentTypesChanged, this, &FilterAndBrowseModel::availableContentTypesChanged);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::contentTypeChanged, this, &FilterAndBrowseModel::contentTypeChanged);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::countChanged, this, &FilterAndBrowseModel::countChanged);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::dataFetched, this, &FilterAndBrowseModel::dataFetched);
    connect(m_replica.data(), &QIfFilterAndBrowseModelReplica::dataChanged, this, &FilterAndBrowseModel::dataChanged);
}
