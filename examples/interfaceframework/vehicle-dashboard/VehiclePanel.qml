// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Example.If.VehicleDashboard

ColumnLayout {
    id: root
    required property VehicleStatus vehicleStatus

    spacing: 8

    Label {
        text: qsTr("Vehicle Status")
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
        implicitHeight: topGrid.implicitHeight + 16
        radius: 6
        color: "#e8e8e8"

        GridLayout {
            id: topGrid
            anchors.fill: parent
            anchors.margins: 8
            columns: 4
            columnSpacing: 8
            rowSpacing: 4

            Label { text: qsTr("Headlights:"); color: "#555"; font.pixelSize: 12 }
            Button {
                text: root.vehicleStatus.headlights ? qsTr("On") : qsTr("Off")
                checkable: true
                checked: root.vehicleStatus.headlights
                onToggled: root.vehicleStatus.headlights = checked
                implicitHeight: 24
                implicitWidth: 48
            }

            Label { text: qsTr("Drive Mode:"); color: "#555"; font.pixelSize: 12 }
            ComboBox {
                model: [qsTr("Eco"), qsTr("Normal"), qsTr("Sport"), qsTr("Off-Road")]
                currentIndex: root.vehicleStatus.driveMode
                onActivated: root.vehicleStatus.driveMode = currentIndex
                implicitHeight: 24
            }

            Label { text: qsTr("VIN:"); color: "#555"; font.pixelSize: 12 }
            Label {
                text: root.vehicleStatus.vin
                color: "#1a1a1a"
                font.family: "monospace"
                font.pixelSize: 11
                Layout.columnSpan: 3
            }

            Label { text: qsTr("Mileage:"); color: "#555"; font.pixelSize: 12 }
            Label {
                text: root.vehicleStatus.mileage.toFixed(1) + " km"
                color: "#1a1a1a"
                font.pixelSize: 12
                Layout.columnSpan: 3
            }
        }
    }

    RowLayout {
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        spacing: 8
        implicitHeight: Math.max(tireCard.implicitHeight, warnCard.implicitHeight)

        Rectangle {
            id: tireCard
            Layout.fillWidth: true
            Layout.fillHeight: true
            implicitHeight: tireCol.implicitHeight + 16
            radius: 6
            color: "#e8e8e8"

            ColumnLayout {
                id: tireCol
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4

                Label { text: qsTr("Tire Pressure (bar)"); color: "#888"; font.pixelSize: 11 }

                GridLayout {
                    columns: 4
                    columnSpacing: 6
                    rowSpacing: 2

                    Label { text: qsTr("FL:"); color: "#555"; font.pixelSize: 11 }
                    Label {
                        text: root.vehicleStatus.tirePressure.frontLeft
                              ? root.vehicleStatus.tirePressure.frontLeft.toFixed(1) : "-"
                        color: "#1a1a1a"; font.pixelSize: 11
                    }
                    Label { text: qsTr("FR:"); color: "#555"; font.pixelSize: 11 }
                    Label {
                        text: root.vehicleStatus.tirePressure.frontRight
                              ? root.vehicleStatus.tirePressure.frontRight.toFixed(1) : "-"
                        color: "#1a1a1a"; font.pixelSize: 11
                    }
                    Label { text: qsTr("RL:"); color: "#555"; font.pixelSize: 11 }
                    Label {
                        text: root.vehicleStatus.tirePressure.rearLeft
                              ? root.vehicleStatus.tirePressure.rearLeft.toFixed(1) : "-"
                        color: "#1a1a1a"; font.pixelSize: 11
                    }
                    Label { text: qsTr("RR:"); color: "#555"; font.pixelSize: 11 }
                    Label {
                        text: root.vehicleStatus.tirePressure.rearRight
                              ? root.vehicleStatus.tirePressure.rearRight.toFixed(1) : "-"
                        color: "#1a1a1a"; font.pixelSize: 11
                    }
                }
            }
        }

        Rectangle {
            id: warnCard
            Layout.fillWidth: true
            Layout.fillHeight: true
            implicitHeight: warnCol.implicitHeight + 16
            radius: 6
            color: "#e8e8e8"

            ColumnLayout {
                id: warnCol
                anchors.fill: parent
                anchors.margins: 8
                spacing: 4

                Label {
                    text: qsTr("Warnings (%1)").arg(
                        root.vehicleStatus.activeWarnings
                        ? root.vehicleStatus.activeWarnings.length : 0)
                    color: "#888"
                    font.pixelSize: 11
                }
                Label {
                    text: {
                        var w = root.vehicleStatus.activeWarnings
                        return (!w || w.length === 0) ? qsTr("None") : w.join(", ")
                    }
                    color: root.vehicleStatus.activeWarnings
                           && root.vehicleStatus.activeWarnings.length > 0
                           ? "#e67e00" : "#888"
                    wrapMode: Text.WordWrap
                    font.pixelSize: 11
                    Layout.fillWidth: true
                }
            }
        }
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.leftMargin: 12
        Layout.rightMargin: 12
        Layout.bottomMargin: 12
        implicitHeight: tripCol.implicitHeight + 16
        radius: 6
        color: "#e8e8e8"

        ColumnLayout {
            id: tripCol
            anchors.fill: parent
            anchors.margins: 8
            spacing: 3

            Label {
                text: qsTr("Trip Log (%1)").arg(
                    root.vehicleStatus.tripLog ? root.vehicleStatus.tripLog.count : 0)
                color: "#888"
                font.pixelSize: 11
            }

            Repeater {
                model: root.vehicleStatus.tripLog

                RowLayout {
                    spacing: 8
                    required property var model

                    Label {
                        text: model.item ? model.item.destination : ""
                        color: "#1a1a1a"
                        font.pixelSize: 11
                        Layout.preferredWidth: 80
                    }
                    Label {
                        text: model.item ? model.item.distance.toFixed(1) + " km" : ""
                        color: "#555"
                        font.pixelSize: 11
                    }
                    Label {
                        text: model.item ? model.item.duration + " min" : ""
                        color: "#555"
                        font.pixelSize: 11
                    }
                }
            }
        }
    }
}
