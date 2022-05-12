/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
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
