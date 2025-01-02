// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QTIFGLOBAL_P_H
#define QTIFGLOBAL_P_H


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


#include <QtCore/qglobal.h>
#include <QtInterfaceFramework/private/qtinterfaceframework-config_p.h>
#include <QtInterfaceFramework/qifqmlconversion_helper.h>
#include <QtInterfaceFramework/qtifglobal.h>

QT_BEGIN_NAMESPACE

#define Q_IF_BACKEND(c, b_f, err) \
Q_D(c); \
auto *backend = b_f; \
if (!backend) { \
    qtif_qmlOrCppWarning(this, QLatin1String(err)); \
    return; \
} \

QT_END_NAMESPACE

#endif // QTIFGLOBAL_P_H

