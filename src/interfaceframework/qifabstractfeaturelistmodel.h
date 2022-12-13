// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFABSTRACTFEATURELISTMODEL_H
#define QIFABSTRACTFEATURELISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtInterfaceFramework/QIfServiceObject>
#include <QtInterfaceFramework/QIfAbstractFeature>

QT_BEGIN_NAMESPACE

class QIfAbstractFeatureListModelPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractFeatureListModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AbstractFeatureListModel)
    QML_UNCREATABLE("")
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QIfAbstractFeature::DiscoveryMode discoveryMode READ discoveryMode WRITE setDiscoveryMode NOTIFY discoveryModeChanged)
    Q_PROPERTY(QIfAbstractFeature::DiscoveryResult discoveryResult READ discoveryResult NOTIFY discoveryResultChanged)
    Q_PROPERTY(QIfServiceObject *serviceObject READ serviceObject WRITE setServiceObject NOTIFY serviceObjectChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY isInitializedChanged)
    Q_PROPERTY(QString error READ errorMessage NOTIFY errorChanged)
    Q_PROPERTY(QString configurationId READ configurationId WRITE setConfigurationId NOTIFY configurationIdChanged REVISION(6, 5))
    Q_PROPERTY(QStringList preferredBackends READ preferredBackends WRITE setPreferredBackends NOTIFY preferredBackendsChanged REVISION(6, 5))

public:

    explicit QIfAbstractFeatureListModel(const QString &interfaceName, QObject *parent = nullptr);

    QIfServiceObject *serviceObject() const;
    QIfAbstractFeature::DiscoveryMode discoveryMode() const;
    QIfAbstractFeature::DiscoveryResult discoveryResult() const;
    bool isValid() const;
    bool isInitialized() const;
    QIfAbstractFeature::Error error() const;
    QString errorMessage() const;
    QString configurationId() const;
    QStringList preferredBackends() const;

public Q_SLOTS:
    bool setServiceObject(QIfServiceObject *so);
    void setDiscoveryMode(QIfAbstractFeature::DiscoveryMode discoveryMode);
    Q_REVISION(6, 5) void setConfigurationId(const QString &configurationId);
    Q_REVISION(6, 5) void setPreferredBackends(const QStringList &preferredBackends);

    QIfAbstractFeature::DiscoveryResult startAutoDiscovery();

Q_SIGNALS:
    void serviceObjectChanged();
    void discoveryModeChanged(QIfAbstractFeature::DiscoveryMode discoveryMode);
    void discoveryResultChanged(QIfAbstractFeature::DiscoveryResult discoveryResult);
    void isValidChanged(bool arg);
    void isInitializedChanged(bool isInitialized);
    void errorChanged(QIfAbstractFeature::Error error, const QString &message);
    Q_REVISION(6, 5) void configurationIdChanged(const QString &configurationId);
    Q_REVISION(6, 5) void preferredBackendsChanged(const QStringList &preferredBackends);

protected:
    QIfAbstractFeatureListModel(QIfAbstractFeatureListModelPrivate &dd, QObject *parent = nullptr);

    virtual bool acceptServiceObject(QIfServiceObject*);
    virtual void connectToServiceObject(QIfServiceObject*);
    virtual void disconnectFromServiceObject(QIfServiceObject*);
    virtual void clearServiceObject() = 0;

     void classBegin() override;
     void componentComplete() override;

    QString interfaceName() const;
    QString errorText() const;
    void setError(QIfAbstractFeature::Error error, const QString &message = QString());

protected Q_SLOTS:
    virtual void onErrorChanged(QIfAbstractFeature::Error error, const QString &message = QString());

private:
    Q_DECLARE_PRIVATE(QIfAbstractFeatureListModel)
    friend class QIfHelperFeature;
    friend class QIfFeatureTester;
};

QT_END_NAMESPACE

#endif // QIFABSTRACTFEATURELISTMODEL_H
