// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
//! [0]
#include <QCoreApplication>

#include "processingservice.h"

#include <QIfRemoteObjectsConfig>

using namespace Qt::StringLiterals;

void serverMain(QIfRemoteObjectsConfig &config)
{
    auto service = new ProcessingService(qApp);
    config.enableRemoting(u"Example.If.RemoteModule"_s, u"ProcessingService"_s, service);
}
//! [0]
