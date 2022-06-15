// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2018 Luxoft GmbH
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef PROCESSINGSERVICE_H
#define PROCESSINGSERVICE_H
//! [0]
#include "rep_processingservice_source.h"

class ProcessingService : public ProcessingServiceSimpleSource
{
public:
    ProcessingService();

    QVariant process(const QString & data) override;
};
//! [0]
#endif // PROCESSINGSERVICE_H
