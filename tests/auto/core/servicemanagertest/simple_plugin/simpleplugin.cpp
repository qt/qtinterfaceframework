// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0


#include "simpleplugin.h"
#include <QThread>

SimplePlugin::SimplePlugin()
    : QObject()
{
}

#include "moc_simpleplugin.cpp"
