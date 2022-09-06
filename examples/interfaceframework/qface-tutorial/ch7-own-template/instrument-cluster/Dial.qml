// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick

Item {
    id: root

    width: 480
    height: 480

    property real value: 0
    property int upDuration: 2000
    property int downDuration: 1000
    property string fillImage: "images/dial_fill_color_left.png"
    property string circleRadius: "0.193"
    property string dialCursor: "images/dial_cursor.png"

    Image {
        id: meter
        property real min: -83.5
        property real max: 157
        width: root.width
        height: width - 1
        rotation: min + (max - min) * root.value
        source: Qt.resolvedUrl(root.dialCursor)
    }
}
