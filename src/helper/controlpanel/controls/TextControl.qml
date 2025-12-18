// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype TextControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Reusable Text control with TextField.
*/
TextField {
    id: root

    property string valueType: "string"  // "string", "int", "real"
    property var typedValue: ControlPanelUtils.parseValue(text, valueType)

    implicitHeight: ControlPanelStyle.controlHeight
    font.pixelSize: ControlPanelStyle.fontSizeBody
    color: ControlPanelStyle.textPrimary

    background: Rectangle {
        color: root.activeFocus ? ControlPanelStyle.backgroundWhite : ControlPanelStyle.backgroundGray
        border.color: root.activeFocus ? ControlPanelStyle.primaryBlue : ControlPanelStyle.borderLight
        border.width: root.activeFocus ? ControlPanelStyle.borderWidthFocus : ControlPanelStyle.borderWidth
        radius: ControlPanelStyle.radius

        Behavior on color {
            ColorAnimation { duration: ControlPanelStyle.animationDuration }
        }

        Behavior on border.color {
            ColorAnimation { duration: ControlPanelStyle.animationDuration }
        }
    }

    leftPadding: ControlPanelStyle.spacingLarge
    rightPadding: ControlPanelStyle.spacingLarge

     // Input validation based on type
    validator: {
        switch (valueType) {
            case "int":
                return intValidator;
            case "real":
                return doubleValidator;
            case "string":
            default:
                return null;
        }
    }

    IntValidator {
        id: intValidator
    }

    DoubleValidator {
        id: doubleValidator
        notation: DoubleValidator.StandardNotation
    }
}
