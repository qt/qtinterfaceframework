// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype TextControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief A text input control for editing string, integer, and real properties.

    TextControl provides a styled text field with built-in input validation
    based on the property type. It is the default control used by the simulation
    control panel for properties that do not have a more specific editor
    (such as enums, booleans, or ranges).

    The control automatically applies the appropriate \l {QtQuick::IntValidator}
    {IntValidator} or \l {QtQuick::DoubleValidator}{DoubleValidator} depending
    on the \l valueType, and exposes a \l typedValue property that returns the
    text content parsed into the correct JavaScript type.

    \section1 Usage

    TextControl inherits from TextField, so all standard TextField properties
    such as \c text, \c placeholderText, and signals such as
    \c editingFinished are available.

    \qml
    TextControl {
        valueType: "int"
        text: backend.speed
        onEditingFinished: backend.setSpeed(text)
    }
    \endqml
*/
TextField {
    id: root

    /*!
        This property defines the expected type for input validation and
        parsing. Accepted values are \c "string", \c "int", and \c "real".

        When set to \c "int", an IntValidator is applied and only integer
        input is accepted. When set to \c "real", a DoubleValidator is
        applied and decimal input is accepted. When set to \c "string"
        (the default), no validator is applied.

        By default, the property is set to \c "string".
    */
    property string valueType: "string"  // "string", "int", "real"

    /*!
        This property holds the current text content parsed into the JavaScript
        type corresponding to \l valueType. For \c "int" it returns an integer,
        for \c "real" a floating-point number, and for \c "string" the raw text
        string.
    */
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
        locale: "C"
    }
}
