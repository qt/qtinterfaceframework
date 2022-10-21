/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtInterfaceFramework module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QIFPLAYQUEUE_H
#define QIFPLAYQUEUE_H

#include <QAbstractListModel>
#include <QtIfMedia/qtifmediaglobal.h>

QT_BEGIN_NAMESPACE

class QIfMediaPlayer;
class QIfPlayQueuePrivate;

class Q_QTIFMEDIA_EXPORT QIfPlayQueue : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int chunkSize READ chunkSize WRITE setChunkSize NOTIFY chunkSizeChanged)
    Q_PROPERTY(int fetchMoreThreshold READ fetchMoreThreshold WRITE setFetchMoreThreshold NOTIFY fetchMoreThresholdChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

    //TODO fix naming
    Q_PROPERTY(QIfPlayQueue::LoadingType loadingType READ loadingType WRITE setLoadingType NOTIFY loadingTypeChanged)
public:
    ~QIfPlayQueue();

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
