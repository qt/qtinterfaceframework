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

//Backport some functions to keep it working with 5.6
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
namespace QtPrivate {
template <typename T> struct QAddConst { typedef const T Type; };
}

// this adds const to non-const objects (like std::as_const)
template <typename T>
Q_DECL_CONSTEXPR typename QtPrivate::QAddConst<T>::Type &qAsConst(T &t) Q_DECL_NOTHROW { return t; }
// prevent rvalue arguments:
template <typename T>
void qAsConst(const T &&) Q_DECL_EQ_DELETE;

#ifndef qUtf16Printable
#  define qUtf16Printable(string) \
    static_cast<const wchar_t*>(static_cast<const void*>(QString(string).utf16()))
#endif
#endif

#define Q_IF_BACKEND(c, b_f, err) \
Q_D(c); \
auto *backend = b_f; \
if (!backend) { \
    qtif_qmlOrCppWarning(this, QLatin1String(err)); \
    return; \
} \

QT_END_NAMESPACE

#endif // QTIFGLOBAL_P_H

