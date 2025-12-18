// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ButtonControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief Reusable styled button.

    Provides a standardized button component with visual feedback for user interactions.
    Supports color customization while maintaining consistent styling across the control panel.
*/

Button {
    id: root

    property color normalColor: ControlPanelStyle.backgroundGray
    property color hoverColor: ControlPanelStyle.hoverGray
    property color pressedColor: ControlPanelStyle.primaryBlueLight
    property color textColor: ControlPanelStyle.textPrimary
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
