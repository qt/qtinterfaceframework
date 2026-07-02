// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework
import Example.If.SmartHome

Window {
    id: root

    visible: true
    width: 420
    height: 560
    title: qsTr("Interface Framework Smart Plug Example")
    color: "#202227"

    property url brokerUrl: "mqtt://localhost:1883"

    //! [configuration]
    InterfaceFrameworkConfiguration {
        id: brokerConfig
        name: "Example.If.SmartHome"
        serviceSettings: ({ "connectionUrl": root.brokerUrl })
    }
    //! [configuration]

    //! [feature]
    SmartPlug {
        id: plug
    }
    //! [feature]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        BrokerBar {
            Layout.fillWidth: true
            currentUrl: root.brokerUrl
            connected: plug.isInitialized
            onBrokerSelected: (brokerUrl) => root.brokerUrl = brokerUrl
        }

        // On/off control
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 170
            radius: 12
            color: plug.on ? "#1f6f43" : "#3a3d44"
            Behavior on color { ColorAnimation { duration: 200 } }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 12

                Label {
                    Layout.alignment: Qt.AlignHCenter
                    text: plug.on ? qsTr("ON") : qsTr("OFF")
                    color: "white"
                    font.pixelSize: 40
                    font.bold: true
                }
                Switch {
                    Layout.alignment: Qt.AlignHCenter
                    checked: plug.on
                    onToggled: plug.on = checked
                }
            }
        }

        // Power metering
        GridLayout {
            Layout.fillWidth: true
            columns: 2
            columnSpacing: 12
            rowSpacing: 12

            component Metric: Rectangle {
                id: tile
                property string label
                property string value
                Layout.fillWidth: true
                Layout.preferredHeight: 90
                radius: 12
                color: "#2b2e35"
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 2
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: tile.value
                        color: "white"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    Label {
                        Layout.alignment: Qt.AlignHCenter
                        text: tile.label
                        color: "#9aa0aa"
                        font.pixelSize: 13
                    }
                }
            }

            Metric { label: qsTr("Voltage"); value: plug.voltage.toFixed(1) + " V" }
            Metric { label: qsTr("Current"); value: plug.current.toFixed(2) + " A" }
            Metric { label: qsTr("Power");   value: (plug.voltage * plug.current).toFixed(1) + " W" }
            Metric { label: qsTr("Energy");  value: plug.energy.toFixed(4) + " kWh" }
        }

        Item { Layout.fillHeight: true }
    }
}
