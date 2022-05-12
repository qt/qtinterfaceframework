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

#ifndef QIFMEDIADEVICEDISCOVERYMODEL_H
#define QIFMEDIADEVICEDISCOVERYMODEL_H

#include <QtInterfaceFramework/QIfAbstractFeatureListModel>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class QIfMediaDeviceDiscoveryModelPrivate;
class QIfMediaDevice;

//TODO Should we add a generic base class ? If not we should use QIfMediaDevice* internally
class Q_QTIFMEDIA_EXPORT QIfMediaDeviceDiscoveryModel : public QIfAbstractFeatureListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:
    enum Roles {
        NameRole = Qt::DisplayRole,
        TypeRole = Qt::UserRole,
        ServiceObjectRole = Qt::UserRole + 1
    };

    explicit QIfMediaDeviceDiscoveryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE QIfMediaDevice *get(int i) const;
    QIfMediaDevice *at(int i) const;

    QHash<int, QByteArray> roleNames() const override;

Q_SIGNALS:
    void countChanged();
    void deviceAdded(QIfMediaDevice *device);
    void deviceRemoved(QIfMediaDevice *device);

protected:
    QIfMediaDeviceDiscoveryModel(QIfMediaDeviceDiscoveryModelPrivate &dd, QObject *parent = nullptr);

    void connectToServiceObject(QIfServiceObject *serviceObject) override;
    void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIfMediaDeviceDiscoveryModel)
    Q_PRIVATE_SLOT(d_func(), void resetModel(const QList<QIfServiceObject *> deviceList))
    Q_PRIVATE_SLOT(d_func(), void onDeviceAdded(QIfServiceObject *device))
    Q_PRIVATE_SLOT(d_func(), void onDeviceRemoved(QIfServiceObject *device))
};

QT_END_NAMESPACE

#endif // QIFMEDIADEVICEDISCOVERYMODEL_H
