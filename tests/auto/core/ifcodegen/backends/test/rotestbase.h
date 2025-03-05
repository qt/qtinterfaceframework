// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef ROTESTBASE_H
#define ROTESTBASE_H

#include "backendstestbase.h"

class RoTestBase : public BackendsTestBase
{
    Q_OBJECT

private slots:
    void testRemoteObjectsConfig();
};

#endif // ROTESTBASE_H
