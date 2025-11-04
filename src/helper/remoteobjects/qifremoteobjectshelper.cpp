// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qifremoteobjectshelper.h"

QT_BEGIN_NAMESPACE

using namespace Qt::Literals::StringLiterals;

/*!
    \namespace QIfRemoteObjectsHelper
    \inmodule QtIfRemoteObjectsHelper

    \brief Contains miscellaneous identifiers and functions.
*/

namespace QIfRemoteObjectsHelper {

/*!
    Returns a default remoteobjects url using the passed \a url.

    The created url is platform dependent.
*/
QString buildDefaultUrl(const QString &url)
{
#ifdef Q_OS_ANDROID
    return u"localabstract:"_s + url;
#elif defined(Q_OS_LINUX)
    static bool isTempWritable = QFileInfo(QDir::tempPath()).isWritable();
    if (!isTempWritable)
        return u"localabstract:"_s + url;
    else
        return u"local:"_s + url;
#else
    return u"local:"_s + url;
#endif
}

} // namespace QIfRemoteObjectsHelper

QT_END_NAMESPACE
