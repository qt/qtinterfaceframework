// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QTIFMEDIAGLOBAL_H
#define QTIFMEDIAGLOBAL_H

#include <QtCore/qglobal.h>
#include <QtIfMedia/qtifmedia-config.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_IFMEDIA_LIB)
#    define Q_QTIFMEDIA_EXPORT Q_DECL_EXPORT
#  else
#    define Q_QTIFMEDIA_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_QTIFMEDIA_EXPORT
#endif

QT_END_NAMESPACE

#endif // QTIFMEDIAGLOBAL_H
