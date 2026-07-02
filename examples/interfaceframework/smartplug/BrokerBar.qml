// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: bar

    property url currentUrl
    property bool connected: false
    signal brokerSelected(url brokerUrl)

    readonly property var presets: [
        { name: qsTr("Local broker"), url: "mqtt://localhost:1883" }
    ]

    implicitHeight: 96
    radius: 12
    color: "#2b2e35"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            Label {
                text: qsTr("MQTT Broker")
                color: "#9aa0aa"
                font.pixelSize: 13
                Layout.fillWidth: true
            }
            Rectangle {
                width: 12
                height: 12
                radius: 6
                color: bar.connected ? "#39d353" : "#d35339"
            }
            Label {
                text: bar.connected ? qsTr("connected") : qsTr("disconnected")
                color: "#9aa0aa"
                font.pixelSize: 12
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 8

            ComboBox {
                id: presetBox
                Layout.preferredWidth: 130
                textRole: "name"
                model: bar.presets
                onActivated: {
                    urlField.text = bar.presets[currentIndex].url
                    bar.brokerSelected(urlField.text)
                }
            }
            TextField {
                id: urlField
                Layout.fillWidth: true
                text: bar.currentUrl
                selectByMouse: true
                onAccepted: bar.brokerSelected(text)
            }
            Button {
                text: qsTr("Connect")
                onClicked: bar.brokerSelected(urlField.text)
            }
        }
    }
}
