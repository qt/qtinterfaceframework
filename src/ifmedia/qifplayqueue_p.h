// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPLAYQUEUE_P_H
#define QIFPLAYQUEUE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qtifmediaglobal_p.h"
#include "private/qabstractitemmodel_p.h"

#include "qifplayqueue.h"
#include "qifplayableitem.h"
#include "qifmediaplayer_p.h"

QT_BEGIN_NAMESPACE

class Q_QTIFMEDIA_EXPORT QIfPlayQueuePrivate : public QAbstractItemModelPrivate
{
public:
    QIfPlayQueuePrivate(QIfMediaPlayer *player, QIfPlayQueue *model);
    ~QIfPlayQueuePrivate() override;

    void initialize();
    void onInitializationDone();
    void onCurrentIndexChanged(int currentIndex);
    void onCanReportCountChanged(bool canReportCount);
    void onDataFetched(const QUuid &identifier, const QList<QVariant> &items, int start, bool moreAvailable);
    void onCountChanged(int new_length);
    void onDataChanged(const QList<QVariant> &data, int start, int count);
    void onFetchMoreThresholdReached();
    void resetModel();
    void clearToDefaults();
    const QIfPlayableItem *itemAt(int i) const;

    QIfMediaPlayerBackendInterface *playerBackend() const;

    QIfPlayQueue * const q_ptr;
    Q_DECLARE_PUBLIC(QIfPlayQueue)

    QIfMediaPlayer *m_player;
    QUuid m_identifier;
    int m_currentIndex;
    int m_chunkSize;
    QList<QVariant> m_itemList;
    bool m_moreAvailable;
    int m_fetchMoreThreshold;
    int m_fetchedDataCount;
    bool m_canReportCount;
    QIfPlayQueue::LoadingType m_loadingType;
};

QT_END_NAMESPACE

#endif // QIFPLAYQUEUE_P_H
