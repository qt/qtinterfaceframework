// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick

Item {
    id: root
    width: 0.37 * 1920
    height: 0.12 * 720

    property real temperature: 0

    Image {
        id: background
        width: parent.width
        height: parent.height
        source: Qt.resolvedUrl("images/top_bar.png")
    }

    Label {
        id: timeText
        anchors.verticalCenter: background.verticalCenter
        anchors.left: background.left
        anchors.leftMargin: 0.2 * background.width

        font.pixelSize: 0.42 * background.height
        font.bold: true

        text: Qt.formatTime(currentDate, "hh:mm")

        property var currentDate: new Date();

        Timer {
            interval: 1000
            repeat: true
            running: true
            onTriggered: {
                timeText.currentDate = new Date();
            }
        }
    }

    Item {
        id: navigator

        width: 0.25 * background.width
        height: background.height
        anchors.verticalCenter: background.verticalCenter
        anchors.horizontalCenter: background.horizontalCenter

        Row {
            id: row
            property int radius: 7
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -16
            spacing: 8

            Repeater {
                model: 3
                delegate: Rectangle {
                    height: row.radius * 2
                    width: row.radius * 2
                    radius: row.radius
                    color: 1 === index ? "white" : "#4d4d4d"
                }
            }
        }
    }

    Label {
        id: temperatureText
        anchors.verticalCenter: background.verticalCenter
        anchors.left: navigator.right
        anchors.leftMargin: 15

        font.pixelSize: 0.42 * background.height
        font.bold: true

        text: root.temperature + "°C"
    }
}
