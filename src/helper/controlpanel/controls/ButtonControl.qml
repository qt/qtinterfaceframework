// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ButtonControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-controls
    \brief A reusable styled button with customizable colors.

    ButtonControl provides a standardized button with hover and press
    visual feedback. It inherits from Button, so all standard Button
    properties (such as \c text and \c onClicked) are available.

    The button colors can be customised via the \l normalColor,
    \l hoverColor, and \l pressedColor properties while keeping
    a consistent look across the simulation control panel.

    \section1 Usage

    \qml
    ButtonControl {
        text: "Add"
        onClicked: backend.addItem()
    }
    \endqml

    \section2 Custom colors
    \qml
    ButtonControl {
        text: "Delete"
        normalColor: "#ffe0e0"
        pressedColor: "#ff8080"
        textColor: "#cc0000"
        onClicked: backend.removeItem(index)
    }
    \endqml
*/

Button {
    id: root

    /*!
        This property holds the background color of the button in its
        normal (idle) state.

        By default, the property is set to
        \l {ControlPanelStyle::backgroundGray}{ControlPanelStyle.backgroundGray}.
    */
    property color normalColor: ControlPanelStyle.backgroundGray

    /*!
        This property holds the background color shown when the user
        hovers over the button.

        By default, the property is set to
        \l {ControlPanelStyle::hoverGray}{ControlPanelStyle.hoverGray}.
    */
    property color hoverColor: ControlPanelStyle.hoverGray

    /*!
        This property holds the background color shown while the button
        is pressed.

        By default, the property is set to
        \l {ControlPanelStyle::primaryBlueLight}{ControlPanelStyle.primaryBlueLight}.
    */
    property color pressedColor: ControlPanelStyle.primaryBlueLight

    /*!
        This property holds the text color of the button in its normal
        and hover states.

        By default, the property is set to
        \l {ControlPanelStyle::textPrimary}{ControlPanelStyle.textPrimary}.
    */
    property color textColor: ControlPanelStyle.textPrimary

    /*!
        This property holds the text color shown while the button is
        pressed.

        By default, the property is set to
        \l {ControlPanelStyle::primaryBlue}{ControlPanelStyle.primaryBlue}.
    */
    property color pressedTextColor: ControlPanelStyle.primaryBlue

    implicitHeight: 36

    contentItem: Label {
        text: root.text
        font.pixelSize: ControlPanelStyle.fontSizeBody
        color: root.down ? root.pressedTextColor : root.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: root.down ? root.pressedColor :
               root.hovered ? root.hoverColor :
               root.normalColor
        border.color: ControlPanelStyle.borderLight
        border.width: ControlPanelStyle.borderWidth
        radius: ControlPanelStyle.radius

        Behavior on color {
            ColorAnimation { duration: ControlPanelStyle.animationDuration }
        }
    }
}
