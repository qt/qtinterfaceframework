// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

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
