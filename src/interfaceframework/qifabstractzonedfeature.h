/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
******************************************************************************/

#ifndef QIFABSTRACTZONEDFEATURE_H
#define QIFABSTRACTZONEDFEATURE_H

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtInterfaceFramework/QIfAbstractFeature>
#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

class QIfZonedFeatureInterface;
class QIfAbstractZonedFeaturePrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractZonedFeature : public QIfAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(QString zone READ zone WRITE setZone NOTIFY zoneChanged)
    Q_PROPERTY(QStringList availableZones READ availableZones NOTIFY availableZonesChanged)
    Q_PROPERTY(QVariantList zones READ zoneFeatureList NOTIFY zonesChanged)
    Q_PROPERTY(QVariantMap zoneAt READ zoneFeatureMap  NOTIFY zonesChanged)

public:

    explicit QIfAbstractZonedFeature(const QString &interface, const QString &zone = QString(), QObject *parent = nullptr);

    QString zone() const;

    QStringList availableZones() const;

    QIfAbstractZonedFeature *zoneAt(const QString &zone) const;
    QList<QIfAbstractZonedFeature*> zones() const;

Q_SIGNALS:
    void availableZonesChanged(const QStringList &zones);
    void zoneChanged();
    void zonesChanged();

protected:
    QIfAbstractZonedFeature(QIfAbstractZonedFeaturePrivate &dd, QObject *parent = nullptr);

    virtual QIfAbstractZonedFeature *createZoneFeature(const QString &zone) = 0;
    QIfZonedFeatureInterface *backend(const QString &interface = QString()) const;

    bool acceptServiceObject(QIfServiceObject *serviceObject) override;
    void connectToServiceObject(QIfServiceObject *serviceObject) override;
    void clearServiceObject() override;

private Q_SLOTS:
    void setZone(const QString &zone);
    void initializeZones(const QStringList &zones);

private:
    QVariantList attributeList() const;
    QVariantMap attributeMap() const;
    QVariantMap zoneFeatureMap() const;
    QVariantList zoneFeatureList() const;

    void resetAttributes();

private:
    Q_DECLARE_PRIVATE(QIfAbstractZonedFeature)
};

QT_END_NAMESPACE

#endif // QIFABSTRACTZONEDFEATURE_H
