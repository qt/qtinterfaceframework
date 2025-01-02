// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFABSTRACTZONEDFEATURE_P_H
#define QIFABSTRACTZONEDFEATURE_P_H

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

#include "qifabstractfeature_p.h"
#include "qifabstractzonedfeature.h"
#include <private/qtifglobal_p.h>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfAbstractZonedFeaturePrivate : public QIfAbstractFeaturePrivate
{
public:
    QIfAbstractZonedFeaturePrivate(const QString &interface, const QString &zone, QIfAbstractFeature *parent);

    QString m_zone;
    QList<QIfAbstractZonedFeature*> m_zoneFeatures;
    QVariantMap m_zoneFeatureMap;
    QVariantList m_zoneFeatureList;
};

QT_END_NAMESPACE

#endif // QIFABSTRACTZONEDFEATURE_P_H
