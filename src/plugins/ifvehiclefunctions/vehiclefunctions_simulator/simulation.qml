// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtInterfaceFramework.VehicleFunctions.simulation

QtObject {
    property var qifclimatecontrol : QIfClimateControlSimulation {
        id: qifclimatecontrol
    }
    property var qifwindowcontrol : QIfConcreteWindowControlSimulation {
        id: qifwindowcontrol
    }
}
