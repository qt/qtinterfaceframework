// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ModelControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief Displays and edits Model data with add, remove, and update operations.
*/

ColumnLayout {
    id: root

    property var modelBackend: null
    property string elementType: "string"
    property var structType: null
    property var enumModel: null

    property int itemCount: 0
    spacing: 8

    Component.onCompleted: {
        if (root.modelBackend) {
            root.itemCount = root.modelBackend.count;
        }
    }

    Connections {
        target: root.modelBackend

        function onCountChanged() {
            root.itemCount = root.modelBackend.count;
        }
    }

    Rectangle {
        Layout.fillWidth: true
        height: 40
        color: ControlPanelStyle.backgroundGray
        border.color: ControlPanelStyle.borderLight
        border.width: 1
        radius: ControlPanelStyle.radius

        RowLayout {
            anchors.fill: parent
            anchors.margins: 8
            spacing: 8

            Label {
                text: "Items"
                font.pixelSize: ControlPanelStyle.fontSizeBody
                color: ControlPanelStyle.textPrimary
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                Layout.preferredWidth: countLabel.width + 16
                Layout.preferredHeight: 24
                color: ControlPanelStyle.primaryBlueLight
                radius: 12

                Label {
                    id: countLabel
                    anchors.centerIn: parent
                    text: root.itemCount.toString()
                    font.pixelSize: ControlPanelStyle.fontSizeSmall
                    font.bold: true
                    color: ControlPanelStyle.primaryBlue
                }
            }

            ButtonControl {
                text: "Add"
                onClicked: addItem()
            }
        }
    }

    ScrollView {
        Layout.fillWidth: true
        Layout.preferredHeight: 300
        clip: true
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width
            spacing: 8

            Repeater {
                id: mainRepeater
                model: root.itemCount

                delegate: Rectangle {
                    id: delegateRoot
                    required property int index

                    Layout.fillWidth: true
                    implicitHeight: itemLayout.implicitHeight + 16
                    color: ControlPanelStyle.backgroundGray
                    border.width: 1
                    radius: ControlPanelStyle.radius

                    RowLayout {
                        id: itemLayout
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 12

                        Rectangle {
                            Layout.preferredWidth: 32
                            Layout.preferredHeight: 32
                            Layout.alignment: Qt.AlignTop
                            color: ControlPanelStyle.primaryBlueLight
                            radius: 16

                            Label {
                                anchors.centerIn: parent
                                text: "#" + (delegateRoot.index + 1)
                                font.pixelSize: ControlPanelStyle.fontSizeSmall
                                font.bold: true
                                color: ControlPanelStyle.primaryBlue
                            }
                        }

                        // Fields
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Repeater {
                                model: root.structType ? root.structType.fields : []

                                delegate: StructField {
                                    required property var modelData

                                    Layout.fillWidth: true

                                    fieldName: modelData.name
                                    fieldType: modelData.type
                                    fieldValue: {
                                        var item = root.modelBackend.at(delegateRoot.index);
                                        return item[modelData.name] ;
                                    }
                                    enumModel: modelData.enumModel || null
                                    listElementType: modelData.listElementType || "string"

                                    onFieldChanged: function(value) {
                                        updateField(delegateRoot.index, modelData.name, value);
                                    }
                                }
                            }
                        }

                        ButtonControl {
                            Layout.alignment: Qt.AlignTop
                            text: ControlPanelStyle.iconClose
                            Layout.preferredWidth: 32
                            Layout.preferredHeight: 32
                            onClicked: removeItem(delegateRoot.index)
                        }
                    }
                }
            }

            Label {
                visible: root.itemCount === 0
                text: "No items in model. Click 'Add' to start."
                color: ControlPanelStyle.textSecondary
                font.italic: true
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 20
            }
        }
    }

    // Functions

    function addItem() {
        if (!root.modelBackend) return;

        // Create new item with default values
        var newItem = {};
        if (root.structType && root.structType.fields) {
            for (var i = 0; i < root.structType.fields.length; i++) {
                var field = root.structType.fields[i];
                newItem[field.name] = ControlPanelUtils.getDefaultValue(field.type);
            }
        }

        // Insert at index 0 (top of list)
        root.modelBackend.insert(0, newItem);
    }

    function removeItem(index) {
        if (!root.modelBackend) return;
        root.modelBackend.remove(index);
    }

    function updateField(index, fieldName, value) {
        if (!root.modelBackend) return;
        // Get current item
        var currentItem = root.modelBackend.at(index);
        if (!currentItem) {
            console.error("Item not found at index", index);
            return;
        }
        var updatedItem = {};
        if (root.structType && root.structType.fields) {
            for (var i = 0; i < root.structType.fields.length; i++) {
                var field = root.structType.fields[i];
                updatedItem[field.name] = currentItem[field.name];
            }
        }
        updatedItem[fieldName] = value;

        root.modelBackend.update(index, updatedItem);
    }
}
