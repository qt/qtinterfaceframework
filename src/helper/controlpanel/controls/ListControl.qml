// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ListControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief Displays and manages list<type> properties with add/remove/edit functionality.

    Supports primitives (int, real, string, bool), enums, and structs.
    Each list item can be expanded (for structs) or edited inline.
    Items are displayed in a scrollable view with index numbers.
 */

ColumnLayout {
    id: root

    // Properties
    property var listValue: []
    property string elementType: "string"
    property var structType: null
    property var enumModel: null
    property var expandedItems: ({})

    signal listChanged(var newList)
    spacing: 8

    // Header
    RowLayout {
        Layout.fillWidth: true

        Label {
            text: "Items (" + listRepeater.count + ")"
            font.bold: true
            font.pixelSize: ControlPanelStyle.fontSizeBody
            color: ControlPanelStyle.textPrimary
        }

        Item { Layout.fillWidth: true }

        ButtonControl {
            text: "Add"
            onClicked: root.addItem()
        }
    }

    // List items
    ScrollView {
        Layout.fillWidth: true
        Layout.preferredHeight: Math.min(listColumn.implicitHeight, 400)
        contentWidth: availableWidth
        clip: true

        ColumnLayout {
            id: listColumn
            width: root.width
            spacing: 4

            Repeater {
                id: listRepeater
                model: root.listValue

                delegate: Rectangle {
                    required property int index
                    required property var modelData

                    Layout.fillWidth: true
                    implicitHeight: itemContent.implicitHeight + 16
                    color: index % 2 === 0 ? ControlPanelStyle.backgroundGray : ControlPanelStyle.backgroundWhite
                    border.color: ControlPanelStyle.borderLight
                    border.width: 1
                    radius: ControlPanelStyle.radius

                    RowLayout {
                        id: itemContent
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8

                        Label {
                            text: "#" + (parent.parent.index + 1)
                            font.pixelSize: ControlPanelStyle.fontSizeSmall
                            color: ControlPanelStyle.textSecondary
                            Layout.preferredWidth: 30
                        }

                        Loader {
                            Layout.fillWidth: true
                            sourceComponent: root.getComponentForType()

                            property var itemValue: parent.parent.modelData
                            property int itemIndex: parent.parent.index
                        }

                        ButtonControl {
                            text: ControlPanelStyle.iconClose
                            Layout.preferredWidth: 30
                            Layout.preferredHeight: 30
                            onClicked: root.removeItem(parent.parent.index)
                        }
                    }
                }
            }
        }
    }

    Label {
        visible: listRepeater.count === 0
        text: "No items. Click 'Add' to start."
        color: ControlPanelStyle.textSecondary
        font.italic: true
        Layout.alignment: Qt.AlignHCenter
    }

    // Components
    Component {
        id: textComponent
        TextControl {
            valueType: root.elementType
            text: itemValue !== undefined ? String(itemValue) : ""
            onEditingFinished: {
                if (typedValue !== itemValue) {
                    root.updateItem(itemIndex, typedValue);
                }
            }
        }
    }

    Component {
        id: boolComponent
        BooleanControl {
            checked: itemValue || false
            onToggled: function(checked) {
                if (checked !== itemValue) {
                    root.updateItem(itemIndex, checked);
                }
            }
        }
    }

    Component {
        id: enumComponent
        ComboBoxControl {
            valueModel: root.enumModel || []
            value: itemValue
            isEnumModel: true
            onValueActivated: function(val) {
                if (val !== itemValue) {
                    root.updateItem(itemIndex, val);
                }
            }
        }
    }

    Component {
        id: structComponent
        StructControl {
            structName: (root.structType ? root.structType.name : "Item") + " #" + (itemIndex + 1)
            structObject: itemValue || {}
            fields: root.structType ? root.structType.fields : []

            // Track expanded state per item
            expanded: root.isItemExpanded(itemIndex)

            onIsExpandedChanged: {
                root.setItemExpanded(itemIndex, expanded);
            }

            onFieldValueChanged: function(fieldName, value) {
                var updated = itemValue || {};
                updated[fieldName] = value;
                root.updateItem(itemIndex, updated);
            }
        }
    }

    // Functions
    function getComponentForType() {
        switch (root.elementType) {
            case "bool":
                return boolComponent;
            case "enum":
                return enumComponent;
            case "struct":
                return structComponent;
            case "int":
            case "real":
            case "string":
            default:
                return textComponent;
        }
    }

    function isItemExpanded(index) {
        return expandedItems[String(index)] === true;
    }

    function setItemExpanded(index, expanded) {
        var newState = {};
        for (var k in expandedItems) {
            newState[k] = expandedItems[k];
        }
        newState[String(index)] = expanded;
        expandedItems = newState;
    }

    // List operations
    function addItem() {
        var newList = listValue.slice();
        var newIndex = newList.length;
        var newValue = ControlPanelUtils.getDefaultValue(
            elementType,
            root.structType ? root.structType.fields : null,
            root.enumModel
        );
        newList.push(newValue);

        // New struct items start expanded
        if (elementType === "struct") {
            setItemExpanded(newIndex, true);
        }

        listChanged(newList);
    }

    function removeItem(index) {
        var newList = listValue.slice();
        newList.splice(index, 1);

        // Rebuild expanded state
        var newState = {};
        for (var k in expandedItems) {
            var idx = parseInt(k);
            if (idx < index) {
                newState[String(idx)] = expandedItems[k];
            } else if (idx > index) {
                newState[String(idx - 1)] = expandedItems[k];
            }
        }
        expandedItems = newState;
        listChanged(newList);
    }

    function updateItem(index, value) {
        var newList = listValue.slice();
        newList[index] = value;
        listChanged(newList);
    }
}
