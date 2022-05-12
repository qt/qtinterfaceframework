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

