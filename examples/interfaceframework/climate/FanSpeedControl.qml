// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls

SpinBox {
    id: control

    from: 0
    to: 5

    up.indicator: Item {
        x: parent.width - width
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40

        IconImage {
            anchors.fill: parent
            source: "assets/air_plus.png"
            color: control.up.pressed ? "lightgrey" : "white"
        }
    }
    down.indicator: Item {
        height: parent.height
        implicitWidth: 40
        implicitHeight: 40

        IconImage {
            anchors.fill: parent
            source: "assets/air_minus.png"
            color: control.down.pressed ? "lightgrey" : "white"
        }
    }

    background: Item {
        implicitWidth: 100
    }
}
