// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype StructField
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief Single field editor for struct fields.

    Renders appropriate control based on field type (string, int, bool, enum, etc.)
    Used by StructControl for editing individual struct fields.
 */

RowLayout {
    id: root

    property string fieldName: ""
    property string fieldType: "string"  // "string", "int", "real", "bool", "list"
    property var fieldValue: ""
    property var enumModel: null
    property string listElementType: "string"

    signal fieldChanged(var value)

    spacing: 12
    implicitHeight: childrenRect.height

    // Field Label
    Label {
        text: root.fieldName + ((root.fieldType === "list" || root.fieldType === "enum")
                                ? " (" + root.fieldType + ")" : "")
        font.pixelSize: ControlPanelStyle.fontSizeBody
        color: ControlPanelStyle.textPrimary
        Layout.preferredWidth: 100
        Layout.alignment: Qt.AlignTop
        topPadding: 6
    }

    // Field Control
    Loader {
        id: controlLoader
        Layout.fillWidth: true
        activeFocusOnTab: true

    sourceComponent: {
        switch (root.fieldType) {
        case "bool":
            return boolComponent
        case "list":
            return listComponent
        case "enum":
            return enumComponent
        default:
            return textComponent
        }
    }
    }

    // Boolean
    Component {
        id: boolComponent
        BooleanControl {
            checked: root.fieldValue
            onToggled: function(checked) {
                if (checked !== root.fieldValue) {
                    root.fieldChanged(checked);
                }
            }
        }
    }

    // Enum
    Component {
        id: enumComponent
        ComboBoxControl {
            valueModel: root.enumModel
            value: root.fieldValue
            isEnumModel: true
            onValueActivated: function(val) {
                if (val !== root.fieldValue) {
                    root.fieldChanged(val);
                }
            }
        }
    }

    // Text (int, real, string)
    Component {
        id: textComponent
        TextControl {
            valueType: root.fieldType
            text: root.fieldValue !== undefined ? String(root.fieldValue) : ""

            onEditingFinished: {
                if (typedValue !== root.fieldValue) {
                    root.fieldChanged(typedValue);
                }
            }
        }
    }

    // List as comma-separated values with brackets
    Component {
        id: listComponent

        RowLayout {
            spacing: 4

            TextControl {
                id: listTextField
                Layout.fillWidth: true
                valueType: "string"
                placeholderText: "Enter comma-separated values (e.g., 123, 456)"

                Component.onCompleted: {
                    text = formatListToString(root.fieldValue);
                }

                Connections {
                    target: root
                    function onFieldValueChanged() {
                        if (!listTextField.activeFocus) {
                            listTextField.text = formatListToString(root.fieldValue);
                        }
                    }
                }

                onEditingFinished: {
                    var newList = parseStringToList(text);
                    if (JSON.stringify(newList) !== JSON.stringify(root.fieldValue)) {
                        root.fieldChanged(newList);
                    }
                }
            }

            Label {
                text: "(" + (root.fieldValue ? root.fieldValue.length : 0) + ")"
                font.pixelSize: ControlPanelStyle.fontSizeSmall
                color: ControlPanelStyle.textSecondary
            }

            function formatListToString(list) {
                if (!list || list.length === 0) {
                    return "";
                }
                return list.join(", ");
            }

            function parseStringToList(str) {
                if (!str || str.trim() === "") {
                    return [];
                }

                var parts = str.split(",");
                var result = [];

                for (var i = 0; i < parts.length; i++) {
                    var trimmed = parts[i].trim();
                    if (trimmed === "") continue;

                    var parsed = ControlPanelUtils.parseValue(trimmed, root.listElementType);
                    result.push(parsed);
                }

                return result;
            }
        }
    }
}
