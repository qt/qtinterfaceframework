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
    ~QIfPlayQueuePrivate();

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
