// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QIFREMOTEOBJECTSHELPER_H
#define QIFREMOTEOBJECTSHELPER_H

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include <QtIfRemoteObjectsHelper/qtifremoteobjectshelper_global.h>

QT_BEGIN_NAMESPACE

namespace QIfRemoteObjectsHelper {

Q_IFREMOTEOBJECTSHELPER_EXPORT QString buildDefaultUrl(const QString &url);

} // namespace QIfRemoteObjectsHelper

QT_END_NAMESPACE

#endif //QIFREMOTEOBJECTSHELPER_H
