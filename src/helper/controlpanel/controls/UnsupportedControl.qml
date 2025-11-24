// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype UnsupportedControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Reusable control for unsupported properties.
*/
Rectangle {
    anchors.fill: parent
    implicitHeight: 60
    color: ControlPanelStyle.backgroundGray
    opacity: 0.6
    radius: 4
    border.color: ControlPanelStyle.borderLight
    border.width: 2

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 4

        Label {
            text: ControlPanelStyle.iconWarning + " Not Implemented"    // warning sign
            font.pixelSize: ControlPanelStyle.fontSizeSmall
            font.bold: true
            color: ControlPanelStyle.textSecondary
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: "This property is not supported in the simulation."
            font.pixelSize: ControlPanelStyle.fontSizeTiny
            color: ControlPanelStyle.textSecondary
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
