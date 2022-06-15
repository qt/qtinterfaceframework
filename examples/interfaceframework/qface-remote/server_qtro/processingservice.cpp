// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2018 Luxoft GmbH
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "processingservice.h"
//! [0]
ProcessingService::ProcessingService()
{
    setLastMessage(QStringLiteral("Service online."));
}

QVariant ProcessingService::process(const QString & data)
{
    setLastMessage(QStringLiteral("Processed data \'%1\'").arg(data));
    return data.length();
}
//! [0]
