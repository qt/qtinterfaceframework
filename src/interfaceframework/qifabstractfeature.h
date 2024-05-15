// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFABSTRACTFEATURE_H
#define QIFABSTRACTFEATURE_H

#include <QtCore/QObject>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlParserStatus>

#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class QIfServiceObject;
class QIfAbstractFeaturePrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractFeature : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AbstractFeature)
    QML_UNCREATABLE("")
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QIfAbstractFeature::DiscoveryMode discoveryMode READ discoveryMode WRITE setDiscoveryMode NOTIFY discoveryModeChanged FINAL)
    Q_PROPERTY(QIfAbstractFeature::DiscoveryResult discoveryResult READ discoveryResult NOTIFY discoveryResultChanged FINAL)
    Q_PROPERTY(QIfServiceObject *serviceObject READ serviceObject WRITE setServiceObject NOTIFY serviceObjectChanged FINAL)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged FINAL)
    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY isInitializedChanged FINAL)
    Q_PROPERTY(QString error READ errorMessage NOTIFY errorChanged FINAL)
    Q_PROPERTY(QString configurationId READ configurationId WRITE setConfigurationId NOTIFY configurationIdChanged REVISION(6, 5) FINAL)
    Q_PROPERTY(QStringList preferredBackends READ preferredBackends WRITE setPreferredBackends NOTIFY preferredBackendsChanged REVISION(6, 5) FINAL)
    Q_PROPERTY(bool backendUpdatesEnabled READ backendUpdatesEnabled WRITE setBackendUpdatesEnabled NOTIFY backendUpdatesEnabledChanged REVISION(6, 8) FINAL)

public:

    enum Error {
        NoError,
        PermissionDenied,
        InvalidOperation,
        Timeout,
        InvalidZone,
        Unknown
    };
    Q_ENUM(Error)

    enum DiscoveryMode {
        InvalidAutoDiscovery = -1,
        NoAutoDiscovery = 0,
        AutoDiscovery,
        LoadOnlyProductionBackends,
        LoadOnlySimulationBackends
    };
    Q_ENUM(DiscoveryMode)

    enum DiscoveryResult {
        NoResult,
        ErrorWhileLoading,
        ProductionBackendLoaded,
        SimulationBackendLoaded
    };
    Q_ENUM(DiscoveryResult)

    explicit QIfAbstractFeature(const QString &interfaceName, QObject *parent = nullptr);
    ~QIfAbstractFeature() override;

    QIfServiceObject *serviceObject() const;
    QIfAbstractFeature::DiscoveryMode discoveryMode() const;
    QIfAbstractFeature::DiscoveryResult discoveryResult() const;
    bool isValid() const;
    bool isInitialized() const;
    QIfAbstractFeature::Error error() const;
    QString errorMessage() const;
    QString configurationId() const;
    QStringList preferredBackends() const;
    bool backendUpdatesEnabled() const;

public Q_SLOTS:
    bool setServiceObject(QIfServiceObject *so);
    void setDiscoveryMode(QIfAbstractFeature::DiscoveryMode discoveryMode);
    Q_REVISION(6, 5) void setConfigurationId(const QString &configurationId);
    Q_REVISION(6, 5) void setPreferredBackends(const QStringList &preferredBackends);
    Q_REVISION(6, 8) void setBackendUpdatesEnabled(bool newBackendUpdatesEnabled);

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
    Q_REVISION(6, 8) void backendUpdatesEnabledChanged(bool backendUpdatesEnabled);

protected:
    QIfAbstractFeature(QIfAbstractFeaturePrivate &dd, QObject *parent = nullptr);

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
    Q_DECLARE_PRIVATE(QIfAbstractFeature)
    Q_PRIVATE_SLOT(d_func(), void onInitializationDone())
    Q_PRIVATE_SLOT(d_func(), void serviceObjectDestroyed())
    friend class QIfFeatureTester;
    friend class QIfHelperFeature;
};

QT_END_NAMESPACE

#endif // QIFABSTRACTFEATURE_H
