// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef LOGGING_H
#define LOGGING_H

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(media)

QT_FORWARD_DECLARE_CLASS(QIfFeatureInterface);

void sqlError(QIfFeatureInterface *interface, const QString &query, const QString &error);

#endif // LOGGING_H
