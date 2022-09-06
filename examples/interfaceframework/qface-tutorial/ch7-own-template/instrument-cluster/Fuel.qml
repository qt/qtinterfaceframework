// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick

Item {
    id: root

    width: 0.73 * 720
    height: width - 1

    property real value: 0

    Item {
        width: root.value * parent.width
        height: parent.height
        clip: true
        Image {
            width: root.width
            height: root.height
            source: Qt.resolvedUrl("images/fuel_level.png")
        }
    }

    Image {
        id: fuel
        anchors.fill: parent
        source: Qt.resolvedUrl("images/fuel.png")

    }
}

