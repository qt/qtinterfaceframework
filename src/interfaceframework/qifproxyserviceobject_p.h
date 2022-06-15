// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFPROXYSERVICEOBJECT_P_H
#define QIFPROXYSERVICEOBJECT_P_H

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

#include <QtCore/QHash>
#include <QtInterfaceFramework/QIfServiceObject>
#include <private/qtifglobal_p.h>

QT_BEGIN_NAMESPACE

class Q_QTINTERFACEFRAMEWORK_EXPORT QIfProxyServiceObjectPrivate
{

public:
    explicit QIfProxyServiceObjectPrivate(QIfServiceInterface *interface);
    explicit QIfProxyServiceObjectPrivate(const QHash<QString, QIfFeatureInterface *> &interfaceMap);

    virtual ~QIfProxyServiceObjectPrivate() = default;

    Q_DISABLE_COPY(QIfProxyServiceObjectPrivate)

    QIfServiceInterface *m_serviceInterface;
    QHash<QString, QIfFeatureInterface*> m_interfaceMap;
};

QT_END_NAMESPACE

#endif // QIFPROXYSERVICEOBJECT_P_H
