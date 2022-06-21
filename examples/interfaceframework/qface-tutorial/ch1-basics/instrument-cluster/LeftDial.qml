// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick

Item {
    id: root

    width: 0.8 * 720
    height: 720

    property real value: 0.0
    property bool metricSystem: false

    Image {
        id: overlay

        width: 0.91 * root.width
        height: 0.98 * width
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        source: Qt.resolvedUrl("images/left_dial.png")
    }

    Text {
        id: speedText

        anchors.verticalCenter: overlay.verticalCenter
        anchors.horizontalCenter: overlay.horizontalCenter
        anchors.verticalCenterOffset: -7
        anchors.horizontalCenterOffset: 5
        font.family: "Source Sans Pro"
        font.pixelSize: 60
        font.letterSpacing: 4
        color: "white"
        text: root.value
    }

    Rectangle {
        width: 60
        height: 1
        opacity: 0.4
        anchors.top: speedText.bottom
        anchors.topMargin: -8
        anchors.horizontalCenter: overlay.horizontalCenter
        anchors.horizontalCenterOffset: 2
    }

    Text {
        id: mph
        anchors.top: speedText.bottom
        anchors.topMargin: -5
        anchors.horizontalCenter: overlay.horizontalCenter
        anchors.horizontalCenterOffset: 2
        font.family: "Source Sans Pro"
        font.pixelSize: 24
        color: "white"
        text: root.metricSystem ? "km/h" : "mph"
    }

    Dial {
        width: 0.66 * root.height
        height: width
        anchors.centerIn: overlay
        anchors.verticalCenterOffset: 2
        fillImage: "images/dial_fill_color_left.png"
        value: root.value / 240
    }
}
