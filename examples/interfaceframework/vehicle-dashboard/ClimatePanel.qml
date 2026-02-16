// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Example.If.VehicleDashboard

ColumnLayout {
    id: root
    required property ClimateControl climateControl

    spacing: 8
    anchors.margins: 12

    Label {
        text: qsTr("Climate Control")
        font.pixelSize: 14
        font.bold: true
        color: "#1a1a1a"
        Layout.topMargin: 12
        Layout.leftMargin: 12
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        implicitHeight: sharedGrid.implicitHeight + 16
        radius: 6
        color: "#e8e8e8"

        GridLayout {
            id: sharedGrid
            anchors.fill: parent
            anchors.margins: 8
            columns: 4
            columnSpacing: 8
            rowSpacing: 4

            Label { text: qsTr("Outside Temp:"); color: "#555"; font.pixelSize: 12 }
            Label {
                text: root.climateControl.outsideTemperature + " \u00B0C"
                color: "#1a1a1a"
                font.bold: true
                font.pixelSize: 12
            }

            Label { text: qsTr("A/C:"); color: "#555"; font.pixelSize: 12 }

            Button {
                text: root.climateControl.airConditioning ? qsTr("On") : qsTr("Off")
                checkable: true
                checked: root.climateControl.airConditioning
                onToggled: root.climateControl.airConditioning = checked
                implicitHeight: 24
                implicitWidth: 48
            }

            Label { text: qsTr("Recirculation:"); color: "#555"; font.pixelSize: 12 }
            ComboBox {
                Layout.columnSpan: 3
                model: [qsTr("Off"), qsTr("On"), qsTr("Auto")]
                currentIndex: root.climateControl.recirculationMode
                onActivated: root.climateControl.recirculationMode = currentIndex
                implicitHeight: 24
            }
        }
    }

    Label {
        text: qsTr("Zone Controls")
        font.pixelSize: 11
        color: "#888"
        Layout.leftMargin: 12
    }

    RowLayout {
        spacing: 0
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        Layout.bottomMargin: 12

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: leftZone.implicitHeight + 16
            radius: 6
            color: "#e8e8e8"

            ZoneClimateColumn {
                id: leftZone
                anchors.fill: parent
                anchors.margins: 8
                zoneName: qsTr("Front Left")
                fanSpeed: root.climateControl.zoneAt.FrontLeft.fanSpeedLevel
                targetTemp: root.climateControl.zoneAt.FrontLeft.targetTemperature
                seatHeating: root.climateControl.zoneAt.FrontLeft.seatHeating
                onFanSpeedModified: function(value) {
                    root.climateControl.zoneAt.FrontLeft.fanSpeedLevel = value
                }
                onTargetTempModified: function(value) {
                    root.climateControl.zoneAt.FrontLeft.targetTemperature = value
                }
                onSeatHeatingModified: function(value) {
                    root.climateControl.zoneAt.FrontLeft.seatHeating = value
                }
            }
        }

        Item { implicitWidth: 8 }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: rightZone.implicitHeight + 16
            radius: 6
            color: "#e8e8e8"

            ZoneClimateColumn {
                id: rightZone
                anchors.fill: parent
                anchors.margins: 8
                zoneName: qsTr("Front Right")
                fanSpeed: root.climateControl.zoneAt.FrontRight.fanSpeedLevel
                targetTemp: root.climateControl.zoneAt.FrontRight.targetTemperature
                seatHeating: root.climateControl.zoneAt.FrontRight.seatHeating
                onFanSpeedModified: function(value) {
                    root.climateControl.zoneAt.FrontRight.fanSpeedLevel = value
                }
                onTargetTempModified: function(value) {
                    root.climateControl.zoneAt.FrontRight.targetTemperature = value
                }
                onSeatHeatingModified: function(value) {
                    root.climateControl.zoneAt.FrontRight.seatHeating = value
                }
            }
        }
    }
}
