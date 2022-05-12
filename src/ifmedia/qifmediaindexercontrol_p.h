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

#ifndef QIFMEDIAINDEXERCONTROL_P_H
#define QIFMEDIAINDEXERCONTROL_P_H

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
#include "private/qifabstractfeature_p.h"

#include "qifmediaindexercontrol.h"
#include "qifmediaindexercontrolbackendinterface.h"

QT_BEGIN_NAMESPACE

class QIfMediaIndexerControlPrivate : public QIfAbstractFeaturePrivate
{
public:
    QIfMediaIndexerControlPrivate(const QString &interface, QIfMediaIndexerControl *parent);

    void clearToDefaults();
    void onProgressChanged(qreal progress);
    void onStateChanged(QIfMediaIndexerControl::State state);

    QIfMediaIndexerControlBackendInterface *indexerBackend() const;

    QIfMediaIndexerControl * const q_ptr;

    qreal m_progress;
    QIfMediaIndexerControl::State m_state;
    Q_DECLARE_PUBLIC(QIfMediaIndexerControl)
};

QT_END_NAMESPACE

#endif // QIFMEDIAINDEXERCONTROL_P_H
