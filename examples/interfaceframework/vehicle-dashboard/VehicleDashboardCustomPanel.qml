// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

// This file is a hand-written wrapper around the auto-generated
// VehicleDashboardControlPanelBase. It injects TirePressureControl as a
// custom editor for the tirePressure property of the VehicleStatus interface.

import QtQuick
import "qrc:///simulation/"

//! [custom-panel]
VehicleDashboardControlPanelBase {
    customVehicleStatusTirepressureComponent: Component {
        TirePressureControl {}
    }
}
//! [custom-panel]
