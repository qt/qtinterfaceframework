// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2018 Luxoft GmbH
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "processingservice.h"

using namespace Qt::StringLiterals;

//! [0]
ProcessingService::ProcessingService()
{
    setLastMessage(u"Service online."_s);
}

QVariant ProcessingService::process(const QString & data)
{
    setLastMessage(u"Processed data \'%1\'"_s.arg(data));
    return data.length();
}
//! [0]
