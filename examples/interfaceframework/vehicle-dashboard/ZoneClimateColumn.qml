// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ColumnLayout {
    id: root

    required property string zoneName
    required property int fanSpeed
    required property int targetTemp
    required property int seatHeating

    signal fanSpeedModified(int value)
    signal targetTempModified(int value)
    signal seatHeatingModified(int value)

    spacing: 4

    Label {
        text: root.zoneName
        font.bold: true
        font.pixelSize: 11
        color: "#1a1a1a"
    }

    GridLayout {
        columns: 2
        columnSpacing: 6
        rowSpacing: 3

        Label { text: qsTr("Fan:"); color: "#555"; font.pixelSize: 11 }
        SpinBox {
            from: 0
            to: 5
            value: root.fanSpeed
            onValueModified: root.fanSpeedModified(value)
            implicitHeight: 24
        }

        Label { text: qsTr("Temp:"); color: "#555"; font.pixelSize: 11 }
        SpinBox {
            from: 16
            to: 30
            value: root.targetTemp
            onValueModified: root.targetTempModified(value)
            implicitHeight: 24

            textFromValue: function(value) {
                return value + " \u00B0C"
            }
        }

        Label { text: qsTr("Seat Heat:"); color: "#555"; font.pixelSize: 11 }
        ComboBox {
            model: [qsTr("Off"), qsTr("Low"), qsTr("Med"), qsTr("High")]
            currentIndex: root.seatHeating
            onActivated: root.seatHeatingModified(currentIndex)
            implicitHeight: 24
            implicitWidth: 80
        }
    }
}
