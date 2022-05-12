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

#ifndef QIFPAGINGMODEL_H
#define QIFPAGINGMODEL_H

#include <QtInterfaceFramework/QIfAbstractFeatureListModel>
#include <QtInterfaceFramework/QtInterfaceFrameworkModule>
#include <QtInterfaceFramework/QIfServiceObject>

QT_BEGIN_NAMESPACE

class QIfPagingModelPrivate;

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfPagingModel : public QIfAbstractFeatureListModel
{
    Q_OBJECT

    Q_PROPERTY(QtInterfaceFrameworkModule::ModelCapabilities capabilities READ capabilities NOTIFY capabilitiesChanged)
    Q_PROPERTY(int chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged)
    Q_PROPERTY(int fetchMoreThreshold READ fetchMoreThreshold WRITE setFetchMoreThreshold NOTIFY fetchMoreThresholdChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

    //TODO fix naming
    Q_PROPERTY(QIfPagingModel::LoadingType loadingType READ loadingType WRITE setLoadingType NOTIFY loadingTypeChanged)

public:

    enum Roles {
        NameRole = Qt::DisplayRole,
        TypeRole = Qt::UserRole,
        ItemRole = Qt::UserRole + 1,
        LastRole = ItemRole
    };

    //TODO fix naming
    enum LoadingType {
        FetchMore,
        DataChanged
    };
    Q_ENUM(LoadingType)

    explicit QIfPagingModel(QObject *parent = nullptr);

    QtInterfaceFrameworkModule::ModelCapabilities capabilities() const;

    int chunkSize() const;
    void setChunkSize(int chunkSize);

    int fetchMoreThreshold() const;
    void setFetchMoreThreshold(int fetchMoreThreshold);

    QIfPagingModel::LoadingType loadingType() const;
    void setLoadingType(QIfPagingModel::LoadingType loadingType);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QVariant get(int index) const;
    Q_INVOKABLE void reload();

    template <typename T> T at(int i) const {
        return data(index(i,0), ItemRole).value<T>();
    }

Q_SIGNALS:
    void capabilitiesChanged(QtInterfaceFrameworkModule::ModelCapabilities capabilities);
    void chunkSizeChanged(int chunkSize);
    void countChanged();
    void fetchMoreThresholdChanged(int fetchMoreThreshold);
    void fetchMoreThresholdReached() const;
    void loadingTypeChanged(QIfPagingModel::LoadingType loadingType);

protected:
    QIfPagingModel(QIfServiceObject *serviceObject, QObject *parent = nullptr);
    QIfPagingModel(QIfPagingModelPrivate &dd, QObject *parent);
    void connectToServiceObject(QIfServiceObject *serviceObject) override;
    void disconnectFromServiceObject(QIfServiceObject *serviceObject) override;
    void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIfPagingModel)
    Q_PRIVATE_SLOT(d_func(), void onCapabilitiesChanged(const QUuid &identifier, QtInterfaceFrameworkModule::ModelCapabilities capabilities))
    Q_PRIVATE_SLOT(d_func(), void onDataFetched(const QUuid &identifer, const QList<QVariant> &items, int start, bool moreAvailable))
    Q_PRIVATE_SLOT(d_func(), void onCountChanged(const QUuid &identifier, int new_length))
    Q_PRIVATE_SLOT(d_func(), void onDataChanged(const QUuid &identifier, const QList<QVariant> &data, int start, int count))
    Q_PRIVATE_SLOT(d_func(), void onFetchMoreThresholdReached())
};

QT_END_NAMESPACE

#endif // QIFPAGINGMODEL_H
