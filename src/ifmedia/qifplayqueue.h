// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPLAYQUEUE_H
#define QIFPLAYQUEUE_H

#include <QtCore/QAbstractListModel>
#include <QtQml/QQmlEngine>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class QIfMediaPlayer;
class QIfPlayQueuePrivate;

class Q_QTIFMEDIA_EXPORT QIfPlayQueue : public QAbstractListModel
{
    Q_OBJECT
    QML_NAMED_ELEMENT(PlayQueue)
    QML_ANONYMOUS

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged)
    Q_PROPERTY(int fetchMoreThreshold READ fetchMoreThreshold WRITE setFetchMoreThreshold NOTIFY fetchMoreThresholdChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

    //TODO fix naming
    Q_PROPERTY(QIfPlayQueue::LoadingType loadingType READ loadingType WRITE setLoadingType NOTIFY loadingTypeChanged)
public:
    ~QIfPlayQueue() override;

    enum Roles {
        NameRole = Qt::DisplayRole,
        TypeRole = Qt::UserRole,
        ItemRole
    };

    //TODO fix naming
    enum LoadingType {
        FetchMore,
        DataChanged
    };
    Q_ENUM(LoadingType)

    int currentIndex() const;
    void setCurrentIndex(int currentIndex);

    int chunkSize() const;
    void setChunkSize(int chunkSize);

    int fetchMoreThreshold() const;
    void setFetchMoreThreshold(int fetchMoreThreshold);

    QIfPlayQueue::LoadingType loadingType() const;
    void setLoadingType(QIfPlayQueue::LoadingType loadingType);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE QVariant get(int index) const;

    template <typename T> T at(int i) const {
        return data(index(i,0), ItemRole).value<T>();
    }

    Q_INVOKABLE void insert(int index, const QVariant &variant);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void move(int cur_index, int new_index);

Q_SIGNALS:
    void chunkSizeChanged(int chunkSize);
    void countChanged();
    void fetchMoreThresholdChanged(int fetchMoreThreshold);
    void fetchMoreThresholdReached() const;
    void loadingTypeChanged(QIfPlayQueue::LoadingType loadingType);

    void currentIndexChanged(int currentIndex);

protected:
    explicit QIfPlayQueue(QIfMediaPlayer *parent = nullptr);

private:
    Q_DECLARE_PRIVATE(QIfPlayQueue)
    Q_PRIVATE_SLOT(d_func(), void onCurrentIndexChanged(int currentIndex))
    Q_PRIVATE_SLOT(d_func(), void onCanReportCountChanged(bool canReportCount))
    Q_PRIVATE_SLOT(d_func(), void onDataFetched(const QUuid &identifier, const QList<QVariant> &items, int start, bool moreAvailable))
    Q_PRIVATE_SLOT(d_func(), void onCountChanged(int new_length))
    Q_PRIVATE_SLOT(d_func(), void onDataChanged(const QList<QVariant> &data, int start, int count))
    Q_PRIVATE_SLOT(d_func(), void onFetchMoreThresholdReached())

    friend class QIfMediaPlayer;
    friend class QIfMediaPlayerPrivate;
};

QT_END_NAMESPACE

#endif // QIFPLAYQUEUE_H
