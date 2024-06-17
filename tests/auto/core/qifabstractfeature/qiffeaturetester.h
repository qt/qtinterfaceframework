// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#ifndef QIFFEATURETESTER_H
#define QIFFEATURETESTER_H

#include <QIfAbstractFeature>
#include <QIfAbstractFeatureListModel>

QT_BEGIN_NAMESPACE

class QIfFeatureTester : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QIfAbstractFeature::DiscoveryMode discoveryMode READ discoveryMode WRITE setDiscoveryMode NOTIFY discoveryModeChanged)
    Q_PROPERTY(QIfAbstractFeature::DiscoveryResult discoveryResult READ discoveryResult NOTIFY discoveryResultChanged)
    Q_PROPERTY(QIfServiceObject *serviceObject READ serviceObject WRITE setServiceObject NOTIFY serviceObjectChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY isInitializedChanged)
    Q_PROPERTY(QString error READ errorMessage NOTIFY errorChanged)
    Q_PROPERTY(bool asynchronousBackendLoading READ asynchronousBackendLoading NOTIFY asynchronousBackendLoadingChanged)

public:
    explicit QIfFeatureTester(QIfAbstractFeature *feature, QObject *parent = nullptr)
        : QObject(parent)
        , m_feature(feature)
        , m_featureListModel(nullptr)
    {
        connect(m_feature, &QIfAbstractFeature::discoveryModeChanged, this, &QIfFeatureTester::discoveryModeChanged);
        connect(m_feature, &QIfAbstractFeature::discoveryResultChanged, this, &QIfFeatureTester::discoveryResultChanged);
        connect(m_feature, &QIfAbstractFeature::serviceObjectChanged, this, &QIfFeatureTester::serviceObjectChanged);
        connect(m_feature, &QIfAbstractFeature::isValidChanged, this, &QIfFeatureTester::isValidChanged);
        connect(m_feature, &QIfAbstractFeature::isInitializedChanged, this, &QIfFeatureTester::isInitializedChanged);
        connect(m_feature, &QIfAbstractFeature::errorChanged, this, &QIfFeatureTester::errorChanged);
        connect(m_feature, &QIfAbstractFeature::asynchronousBackendLoadingChanged, this, &QIfFeatureTester::asynchronousBackendLoadingChanged);
    }

    explicit QIfFeatureTester(QIfAbstractFeatureListModel *featureModel, QObject *parent = nullptr)
        : QObject(parent)
        , m_feature(nullptr)
        , m_featureListModel(featureModel)
    {
        connect(m_featureListModel, &QIfAbstractFeatureListModel::discoveryModeChanged, this, &QIfFeatureTester::discoveryModeChanged);
        connect(m_featureListModel, &QIfAbstractFeatureListModel::discoveryResultChanged, this, &QIfFeatureTester::discoveryResultChanged);
        connect(m_featureListModel, &QIfAbstractFeatureListModel::serviceObjectChanged, this, &QIfFeatureTester::serviceObjectChanged);
        connect(m_featureListModel, &QIfAbstractFeatureListModel::isValidChanged, this, &QIfFeatureTester::isValidChanged);
        connect(m_featureListModel, &QIfAbstractFeatureListModel::isInitializedChanged, this, &QIfFeatureTester::isInitializedChanged);
        connect(m_featureListModel, &QIfAbstractFeatureListModel::errorChanged, this, &QIfFeatureTester::errorChanged);
        connect(m_featureListModel, &QIfAbstractFeatureListModel::asynchronousBackendLoadingChanged, this, &QIfFeatureTester::asynchronousBackendLoadingChanged);
    }

    QIfServiceObject *serviceObject() const
    {
        return m_feature ? m_feature->serviceObject() : m_featureListModel->serviceObject();
    }

    QIfAbstractFeature::DiscoveryMode discoveryMode() const
    {
        return m_feature ? m_feature->discoveryMode() : m_featureListModel->discoveryMode();
    }

    QIfAbstractFeature::DiscoveryResult discoveryResult() const
    {
        return m_feature ? m_feature->discoveryResult() : m_featureListModel->discoveryResult();
    }

    bool isValid() const
    {
        return m_feature ? m_feature->isValid() : m_featureListModel->isValid();
    }

    bool isInitialized() const
    {
        return m_feature ? m_feature->isInitialized() : m_featureListModel->isInitialized();
    }

    QIfAbstractFeature::Error error() const
    {
        return m_feature ? m_feature->error() : m_featureListModel->error();
    }

    QString errorMessage() const
    {
        return m_feature ? m_feature->errorMessage() : m_featureListModel->errorMessage();
    }

    QString errorText() const
    {
        return m_feature ? m_feature->errorText() : m_featureListModel->errorText();
    }

    bool asynchronousBackendLoading() const
    {
        return m_feature ? m_feature->asynchronousBackendLoading() : m_featureListModel->asynchronousBackendLoading();
    }

    void setBackendUpdatesEnabled(bool backendUpdatesEnabled)
    {
        m_feature ? m_feature->setBackendUpdatesEnabled(backendUpdatesEnabled) : m_featureListModel->setBackendUpdatesEnabled(backendUpdatesEnabled);
    }

    int intProperty() const
    {
        return m_feature ? m_feature->property("intProperty").toInt() : m_featureListModel->property("intProperty").toInt();
    }

public Q_SLOTS:
    bool setServiceObject(QIfServiceObject *so)
    {
        return m_feature ? m_feature->setServiceObject(so) : m_featureListModel->setServiceObject(so);
    }
    void setDiscoveryMode(QIfAbstractFeature::DiscoveryMode discoveryMode)
    {
        return m_feature ? m_feature->setDiscoveryMode(discoveryMode) : m_featureListModel->setDiscoveryMode(discoveryMode);
    }
    QIfAbstractFeature::DiscoveryResult startAutoDiscovery()
    {
        return m_feature ? m_feature->startAutoDiscovery() : m_featureListModel->startAutoDiscovery();
    }

Q_SIGNALS:
    void serviceObjectChanged();
    void discoveryModeChanged(QIfAbstractFeature::DiscoveryMode discoveryMode);
    void discoveryResultChanged(QIfAbstractFeature::DiscoveryResult discoveryResult);
    void isValidChanged(bool arg);
    void isInitializedChanged(bool isInitialized);
    void errorChanged(QIfAbstractFeature::Error error, const QString &message);
    void asynchronousBackendLoadingChanged(bool asynchronousBackendLoading);

private:
    QIfAbstractFeature *m_feature;
    QIfAbstractFeatureListModel *m_featureListModel;
};

QT_END_NAMESPACE

#endif // QIFFEATURETESTER_H
