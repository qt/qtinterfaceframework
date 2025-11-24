// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype RangeControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Reusable Range control with Slider.
*/
ColumnLayout {
    id: root

    property real from: ControlPanelStyle.minInt32
    property real to: ControlPanelStyle.maxInt32
    property real value: 0
    property real stepSize: 1
    property alias enabled: slider.enabled
    signal moved(real value)
    spacing: 6
    implicitHeight: childrenRect.height

    RowLayout {
        Layout.fillWidth: true

        Label {
            text: root.from
            font.pixelSize: ControlPanelStyle.fontSizeCaption
            color: ControlPanelStyle.textSecondary
        }

        Item { Layout.fillWidth: true }

        Label {
            text: String(root.value)
            font.pixelSize: 13
            font.bold: true
            color: ControlPanelStyle.primaryBlue
        }

        Item { Layout.fillWidth: true }

        Label {
            text: root.to
            font.pixelSize: ControlPanelStyle.fontSizeCaption
            color: ControlPanelStyle.textSecondary
        }
    }

    Slider {
        id: slider
        Layout.fillWidth: true

        from: root.from
        to: root.to
        value: root.value
        stepSize: root.stepSize
        snapMode: root.stepSize > 0 ? Slider.SnapAlways : Slider.NoSnap

        onMoved: {
            root.moved(value)
        }

        background: Rectangle {
            x: slider.leftPadding
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            width: slider.availableWidth
            height: 4
            radius: 2
            color: ControlPanelStyle.borderLight

            Rectangle {
                width: slider.visualPosition * parent.width
                height: parent.height
                color: ControlPanelStyle.primaryBlue
                radius: 2
            }
        }

        handle: Rectangle {
            x: slider.leftPadding + slider.visualPosition * (slider.availableWidth - width)
            y: slider.topPadding + slider.availableHeight / 2 - height / 2
            width: 16
            height: 16
            radius: 8
            color: slider.pressed ? ControlPanelStyle.primaryBlueDark : ControlPanelStyle.primaryBlue
            border.color: ControlPanelStyle.backgroundWhite
            border.width: 2
        }
    }
}
