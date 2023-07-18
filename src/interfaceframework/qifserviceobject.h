// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFSERVICEOBJECT_H
#define QIFSERVICEOBJECT_H

#include <QtCore/QObject>
#include <QtInterfaceFramework/QIfServiceInterface>
#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfServiceObject : public QObject, public QIfServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QIfServiceInterface)

    Q_PROPERTY(QString id READ id CONSTANT FINAL)
    Q_PROPERTY(QString configurationId READ configurationId CONSTANT FINAL REVISION(6, 5))
    Q_PROPERTY(QVariantMap serviceSettings READ serviceSettings WRITE updateServiceSettings NOTIFY serviceSettingsChanged FINAL REVISION(6, 5))

public:
    explicit QIfServiceObject(QObject *parent = nullptr);

    virtual QString id() const override;

    const QVariantMap &serviceSettings() const;
    void updateServiceSettings(const QVariantMap &settings) override;

Q_SIGNALS:
    Q_REVISION(6, 5) void serviceSettingsChanged();

private:
    QString m_id;
    QVariantMap m_serviceSettings;
};

QT_END_NAMESPACE

#endif // QIFSERVICEOBJECT_H
