// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Window

Window {
    id: root

    width: 1920
    height: 720
    title: qsTr("QtIF Instrument Cluster")
    visible: true
    color: "#0c0c0c"

    LeftDial {
        id: leftDial
        anchors.left: parent.left
        anchors.leftMargin: 0.1 * width

        value: 80
    }

    RightDial {
        id: rightDial
        anchors.right: parent.right
        anchors.rightMargin: 0.1 * width

        value: 3000
    }

    Top {
        id: topbar
        y: 7
        anchors.horizontalCenter: parent.horizontalCenter

        temperature: 15
    }

    Image {
        anchors.fill: parent
        source: Qt.resolvedUrl("images/mask_overlay.png")
    }
}
