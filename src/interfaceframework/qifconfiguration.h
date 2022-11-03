// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFCONFIGURATION_H
#define QIFCONFIGURATION_H

#include <QtCore/QObject>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlParserStatus>

#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class QIfConfigurationPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfConfiguration : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_DISABLE_COPY(QIfConfiguration)
    QML_NAMED_ELEMENT(InterfaceFrameworkConfiguration)
    QML_ADDED_IN_MINOR_VERSION(5)
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(bool valid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariantMap serviceSettings READ serviceSettings WRITE setServiceSettings NOTIFY serviceSettingsChanged)
    Q_PROPERTY(QString simulationFile READ simulationFile WRITE setSimulationFile NOTIFY simulationFileChanged)
    Q_PROPERTY(QString simulationDataFile READ simulationDataFile WRITE setSimulationDataFile NOTIFY simulationDataFileChanged)

public:
    explicit QIfConfiguration(const QString &name = QString(), QObject *parent = nullptr);
    ~QIfConfiguration() override;

    bool isValid() const;
    QString name() const;
    QVariantMap serviceSettings() const;
    QString simulationFile() const;
    QString simulationDataFile() const;

public Q_SLOTS:
    bool setName(const QString &name);
    bool setServiceSettings(const QVariantMap &serviceSettings);
    bool setSimulationFile(const QString &simulationFile);
    bool setSimulationDataFile(const QString &simulationDataFile);

Q_SIGNALS:
    void isValidChanged(bool isValid);
    void nameChanged(const QString &name);
    void serviceSettingsChanged(const QVariantMap &serviceSettings);
    void simulationFileChanged(const QString &simulationFile);
    void simulationDataFileChanged(const QString &simulationDataFile);

public: //static methods
    static bool exists(const QString &group);

    static QVariantMap serviceSettings(const QString &group);
    static bool setServiceSettings(const QString &group, const QVariantMap &serviceSettings);
    static bool areServiceSettingsSet(const QString &group);

    static QString simulationFile(const QString &group);
    static bool setSimulationFile(const QString &group, const QString &simulationFile);
    static bool isSimulationFileSet(const QString &group);

    static QString simulationDataFile(const QString &group);
    static bool setSimulationDataFile(const QString &group, const QString &simulationDataFile);
    static bool isSimulationDataFileSet(const QString &group);

protected:
    QIfConfiguration(QIfConfigurationPrivate &dd, QObject *parent);

    void classBegin() override;
    void componentComplete() override;

private:
    Q_DECLARE_PRIVATE(QIfConfiguration)
};

QT_END_NAMESPACE

#endif // QIFCONFIGURATION_H

