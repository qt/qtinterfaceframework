// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFGLOBAL_H
#define QIFGLOBAL_H

#include <QtCore/qglobal.h>
#include <QtInterfaceFramework/qtinterfaceframework-config.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_INTERFACEFRAMEWORK_LIB)
#    define Q_QTINTERFACEFRAMEWORK_EXPORT Q_DECL_EXPORT
#  else
#    define Q_QTINTERFACEFRAMEWORK_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_QTINTERFACEFRAMEWORK_EXPORT
#endif

QT_END_NAMESPACE

#endif // QIFGLOBAL_H

