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

#include "logging.h"

#include <QtInterfaceFramework/QIfFeatureInterface>

Q_LOGGING_CATEGORY(media, "qt.if.media.media_simulator")

void sqlError(QIfFeatureInterface *interface, const QString &query, const QString &error)
{
    qCWarning(media) << "Error when executing SQL statement:" << query;
    qCWarning(media) << "ERROR:" << error;
    emit interface->errorChanged(QIfAbstractFeature::Unknown, error);
}
