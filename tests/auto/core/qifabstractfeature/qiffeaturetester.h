/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

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

private:
    QIfAbstractFeature *m_feature;
    QIfAbstractFeatureListModel *m_featureListModel;
};

QT_END_NAMESPACE

#endif // QIFFEATURETESTER_H
