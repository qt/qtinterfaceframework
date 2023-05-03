// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
//! [0]
#include <QCoreApplication>

#include "processingservice.h"
#include "core.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    ProcessingService service;
    Core::instance()->host()->enableRemoting(&service,u"Example.If.RemoteModule.ProcessingService"_s);

    return app.exec();
}
//! [0]
