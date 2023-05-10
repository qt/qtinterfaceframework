// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "echobackend.h"
#include "echozonedbackend.h"
#include "echomodulesimulatorplugin.h"

extern QVector<QIfFeatureInterface *> echoInterfaceBuilder(EchomoduleSimulatorPlugin *plugin)
{
    QVector<QIfFeatureInterface *> res;
    res << new EchoBackend(plugin);
    res << new EchoZonedBackend(plugin);
    return res;
}
