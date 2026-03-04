// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype SpinControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-controls
    \brief A numeric spin box control for editing bounded integer properties.

    \techpreview

    SpinControl provides a styled SpinBox with increment and decrement
    buttons and optional minimum/maximum labels displayed above the input.
    It is used by the simulation control panel for properties annotated
    with \c {@range}, \c {@minimum}, or \c {@maximum} in the QFace IDL.

    The spin box is editable by default, allowing the user to type a value
    directly or use the \c + and \c - buttons.

    \section1 Usage

    \qml
    SpinControl {
        from: 0
        to: 255
        value: backend.brightness
        onValueModified: function(value) {
            backend.setBrightness(value)
        }
    }
    \endqml
*/
ColumnLayout {
    id: root

    /*!
        This property holds the minimum allowed value.

        By default, the property is set to the minimum 32-bit signed
        integer value (\c {-2147483648}).
    */
    property int from: ControlPanelStyle.minInt32

    /*!
        This property holds the maximum allowed value.

        By default, the property is set to the maximum 32-bit signed
        integer value (\c {2147483647}).
    */
    property int to: ControlPanelStyle.maxInt32

    /*!
        This property holds the current value of the spin box.

        By default, the property is set to \c 0.
    */
    property int value: 0

    /*!
        This property holds whether the user can type a value directly
        into the spin box.

        By default, the property is set to \c true.
    */
    property alias editable: spinBox.editable

    /*!
        This property controls whether the minimum and maximum labels
        are displayed above the spin box.

        By default, the property is set to \c true.
    */
    property bool showMinMaxLabels: true

    /*!
        \qmlsignal SpinControl::valueModified(int value)

        This signal is emitted when the user modifies the value, either
        by typing, clicking the \c + / \c - buttons, or using the
        keyboard arrows. The \a value parameter contains the new value.
    */
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
