// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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
    QML_NAMED_ELEMENT(PagingModel)

    Q_PROPERTY(QtInterfaceFrameworkModule::ModelCapabilities capabilities READ capabilities NOTIFY capabilitiesChanged FINAL)
    Q_PROPERTY(int chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged FINAL)
    Q_PROPERTY(int fetchMoreThreshold READ fetchMoreThreshold WRITE setFetchMoreThreshold NOTIFY fetchMoreThresholdChanged FINAL)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged FINAL)

    //TODO fix naming
    Q_PROPERTY(QIfPagingModel::LoadingType loadingType READ loadingType WRITE setLoadingType NOTIFY loadingTypeChanged FINAL)

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
    Q_PRIVATE_SLOT(d_func(), void onCapabilitiesChanged(QUuid identifier, QtInterfaceFrameworkModule::ModelCapabilities capabilities))
    Q_PRIVATE_SLOT(d_func(), void onDataFetched(QUuid identifer, const QList<QVariant> &items, int start, bool moreAvailable))
    Q_PRIVATE_SLOT(d_func(), void onCountChanged(QUuid identifier, int new_length))
    Q_PRIVATE_SLOT(d_func(), void onDataChanged(QUuid identifier, const QList<QVariant> &data, int start, int count))
    Q_PRIVATE_SLOT(d_func(), void onFetchMoreThresholdReached())
};

QT_END_NAMESPACE

#endif // QIFPAGINGMODEL_H
