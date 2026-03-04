// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ListControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-controls
    \brief An editor for \c {list<type>} properties with add, remove, and
    edit functionality.

    \techpreview

    ListControl displays a scrollable list of items with numbered indices.
    It supports primitives (\c int, \c real, \c string, \c bool), enums,
    and structs. Each item is rendered using the appropriate control based
    on \l elementType, and struct items can be expanded inline.

    \section1 Usage

    \qml
    ListControl {
        elementType: "int"
        listValue: backend.sensorReadings
        onListChanged: function(newList) {
            backend.setSensorReadings(newList)
        }
    }
    \endqml

    \section2 With structs
    \qml
    ListControl {
        elementType: "struct"
        listValue: backend.waypoints
        structType: {
            "name": "Waypoint",
            "fields": [
                { name: "lat", type: "real" },
                { name: "lon", type: "real" }
            ]
        }
        onListChanged: function(newList) {
            backend.setWaypoints(newList)
        }
    }
    \endqml
*/

ColumnLayout {
    id: root

    /*!
        This property holds the JavaScript array of list items.
    */
    property var listValue: []

    /*!
        This property defines the type of each list element. Accepted
        values are \c "string", \c "int", \c "real", \c "bool",
        \c "enum", and \c "struct".
    */
    property string elementType: "string"

    /*!
        This property holds the struct descriptor used when
        \l elementType is \c "struct". It should be a JavaScript object
        with \c name and \c fields keys.
    */
    property var structType: null

    /*!
        This property holds the enum model used when \l elementType is
        \c "enum".
    */
    property var enumModel: null

    property var expandedItems: ({})

    /*!
        \qmlsignal ListControl::listChanged(var newList)

        This signal is emitted when items are added, removed, or
        modified. The \a newList parameter contains the updated array.
    */
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
