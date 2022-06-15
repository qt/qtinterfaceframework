// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFREMOTEOBJECTSHELPER_H
#define QIFREMOTEOBJECTSHELPER_H

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

QT_BEGIN_NAMESPACE

class QIfRemoteObjectsHelper {

public:
    static QString buildDefaultUrl(const QString &url) {
#ifdef Q_OS_ANDROID
        return QStringLiteral("localabstract:") + url;
#elif defined(Q_OS_LINUX)
        static bool isTempWritable = QFileInfo(QDir::tempPath()).isWritable();
        if (!isTempWritable)
            return QStringLiteral("localabstract:") + url;
        else
            return QStringLiteral("local:") + url;
#else
        return QStringLiteral("local:") + url;
#endif
    }
};

QT_END_NAMESPACE

#endif //QIFREMOTEOBJECTSHELPER_H
