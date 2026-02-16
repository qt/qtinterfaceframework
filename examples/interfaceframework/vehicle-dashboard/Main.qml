// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Example.If.VehicleDashboard

Window {
    id: window

    visible: true
    width: 520
    height: 400
    minimumWidth: 400
    minimumHeight: 320
    title: qsTr("Vehicle Dashboard")
    color: "#f5f5f5"

    ClimateControl {
        id: climateCtrl
    }

    VehicleStatus {
        id: vehicleStatus
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 0
        spacing: 0

        TabBar {
            id: tabBar
            Layout.fillWidth: true

            TabButton { text: qsTr("Climate") }
            TabButton { text: qsTr("Vehicle") }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: tabBar.currentIndex

            ScrollView {
                clip: true
                contentWidth: availableWidth

                ClimatePanel {
                    width: parent.width
                    climateControl: climateCtrl
                }
            }

            ScrollView {
                clip: true
                contentWidth: availableWidth

                VehiclePanel {
                    width: parent.width
                    vehicleStatus: vehicleStatus
                }
            }
        }
    }
}
