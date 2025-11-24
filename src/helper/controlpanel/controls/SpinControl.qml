// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype SpinControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Reusable Spin control with SpinBox.
*/
ColumnLayout {
    id: root

    property int from: ControlPanelStyle.minInt32
    property int to: ControlPanelStyle.maxInt32
    property int value: 0
    property alias enabled: spinBox.enabled
    property alias editable: spinBox.editable
    property bool showMinMaxLabels: true
    signal valueModified(int value)
    spacing: 4
    implicitHeight: childrenRect.height

    RowLayout {
        visible: root.showMinMaxLabels
        Layout.fillWidth: true

        Label {
            text: "Min: " + root.from
            font.pixelSize: ControlPanelStyle.fontSizeTiny
            color: ControlPanelStyle.textSecondary
        }

        Item { Layout.fillWidth: true }

        Label {
            text: "Max: " + root.to
            font.pixelSize: ControlPanelStyle.fontSizeTiny
            color: ControlPanelStyle.textSecondary
        }
    }

    SpinBox {
        id: spinBox
        Layout.fillWidth: true
        Layout.preferredHeight: ControlPanelStyle.controlHeight

        from: root.from
        to: root.to
        value: root.value
        editable: true

        validator: IntValidator {
            bottom: spinBox.from
            top: spinBox.to
        }

        onValueModified: {
            root.valueModified(value)
        }

        background: Rectangle {
            color: spinBox.down ? ControlPanelStyle.primaryBlueLight :
                   (spinBox.hovered ? ControlPanelStyle.hoverGray : ControlPanelStyle.backgroundGray)
            border.color: spinBox.activeFocus ? ControlPanelStyle.primaryBlue : ControlPanelStyle.borderLight
            border.width: spinBox.activeFocus ? ControlPanelStyle.borderWidthFocus : ControlPanelStyle.borderWidth
            radius: ControlPanelStyle.radius

            Behavior on color {
                ColorAnimation { duration: ControlPanelStyle.animationDuration }
            }
        }

        contentItem: TextInput {
            text: spinBox.textFromValue(spinBox.value, spinBox.locale)
            font: spinBox.font
            color: ControlPanelStyle.textPrimary
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            validator: spinBox.validator
            inputMethodHints: Qt.ImhDigitsOnly
        }

        up.indicator: Rectangle {
            x: parent.width - width
            height: parent.height
            implicitWidth: 32
            implicitHeight: 32
            color: spinBox.up.pressed ? ControlPanelStyle.primaryBlueLight :
                   (spinBox.up.hovered ? ControlPanelStyle.primaryBlueDark : "transparent")

            Label {
                anchors.centerIn: parent
                text: "+"
                font.pixelSize: ControlPanelStyle.fontSizeH1
                color: ControlPanelStyle.textPrimary
            }
        }

        down.indicator: Rectangle {
            x: 0
            height: parent.height
            implicitWidth: 32
            implicitHeight: 32
            color: spinBox.down.pressed ? ControlPanelStyle.primaryBlueLight :
                   (spinBox.down.hovered ? ControlPanelStyle.primaryBlueDark : "transparent")

            Label {
                anchors.centerIn: parent
                text: "-"
                font.pixelSize: ControlPanelStyle.fontSizeH1
                color: ControlPanelStyle.textPrimary
            }
        }
    }
}
