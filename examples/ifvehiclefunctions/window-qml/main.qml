// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import QtInterfaceFramework.VehicleFunctions

ApplicationWindow {
    title: "Window Control"
    visible: true
    width: mainLayout.width
    height: mainLayout.height

    WindowControl {
        id: windowControl
        discoveryMode: WindowControl.LoadOnlySimulationBackends
    }

    ColumnLayout {
        id: mainLayout
        WindowItem {
            title: "Roof"
            Layout.alignment: Qt.AlignHCenter
            zone: windowControl.zoneAt.Roof
        }

        Grid {
            id: grid
            columns: 2
            rows: 2
            WindowItem {
                title: "Front Left Zone"
                zone: windowControl.zoneAt.FrontLeft
            }
            WindowItem {
                title: "Front Right Zone"
                zone: windowControl.zoneAt.FrontRight
            }
            WindowItem {
                title: "Rear Left Zone"
                zone: windowControl.zoneAt.RearLeft
            }
            WindowItem {
                title: "Rear Right Zone"
                zone: windowControl.zoneAt.RearRight
            }
        }

        WindowItem {
            title: "Rear"
            Layout.alignment: Qt.AlignHCenter
            zone: windowControl.zoneAt.Rear
        }
    }

    Dialog {
        id: messageDialog
        anchors.centerIn: parent
        title: "Auto Discovery Failed !"
        standardButtons: Dialog.Ok

        Label {
            text: "No WindowControl Backend available"
        }
    }

    Component.onCompleted: {
        if (!windowControl.isValid)
            messageDialog.open()
    }
}
