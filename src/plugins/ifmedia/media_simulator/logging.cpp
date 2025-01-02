// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "logging.h"

#include <QtInterfaceFramework/QIfFeatureInterface>

Q_LOGGING_CATEGORY(media, "qt.if.media.media_simulator")

void sqlError(QIfFeatureInterface *interface, const QString &query, const QString &error)
{
    qCWarning(media) << "Error when executing SQL statement:" << query;
    qCWarning(media) << "ERROR:" << error;
    emit interface->errorChanged(QIfAbstractFeature::Unknown, error);
}
