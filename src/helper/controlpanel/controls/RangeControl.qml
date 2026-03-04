// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype RangeControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-controls
    \brief A slider control for editing bounded numeric properties.

    \techpreview

    RangeControl provides a styled Slider with labels that display the
    minimum, current, and maximum values. It is used by the simulation
    control panel for properties annotated with \c {@range} in the
    QFace IDL.

    The slider supports discrete steps via the \l stepSize property
    and snaps to the nearest step when \l stepSize is greater than
    zero.

    \section1 Usage

    \qml
    RangeControl {
        from: 0
        to: 100
        stepSize: 5
        value: backend.volume
        onMoved: function(value) {
            backend.setVolume(value)
        }
    }
    \endqml
*/
ColumnLayout {
    id: root

    /*!
        This property holds the minimum allowed value of the slider.

        By default, the property is set to the minimum 32-bit signed
        integer value (\c {-2147483648}).
    */
    property real from: ControlPanelStyle.minInt32

    /*!
        This property holds the maximum allowed value of the slider.

        By default, the property is set to the maximum 32-bit signed
        integer value (\c {2147483647}).
    */
    property real to: ControlPanelStyle.maxInt32

    /*!
        This property holds the current value of the slider.

        By default, the property is set to \c 0.
    */
    property real value: 0

    /*!
        This property holds the step size for the slider. When set to
        a value greater than zero, the slider snaps to the nearest
        multiple of \c stepSize.

        By default, the property is set to \c 1.
    */
    property real stepSize: 1

    /*!
        \qmlsignal RangeControl::moved(real value)

        This signal is emitted when the user moves the slider handle.
        The \a value parameter contains the new slider value.
    */
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
