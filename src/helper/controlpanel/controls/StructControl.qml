// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype StructControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief Displays struct properties.

    Shows field count badge and renders individual fields using StructField.
    Supports nested lists, enums, and all primitive types.
    Emits fieldValueChanged signal when any field is modified.
 */

Rectangle {
    id: root

    property string structName: ""
    property var structObject: null
    property var fields: []
    property bool expanded: false

    signal fieldValueChanged(string fieldName, var value)
    signal isExpandedChanged()

    implicitHeight: headerArea.height + (expanded ? fieldsArea.implicitHeight + 16 : 0)
    color: ControlPanelStyle.backgroundWhite
    border.color: ControlPanelStyle.borderLight
    border.width: 1
    radius: ControlPanelStyle.radius

    Behavior on implicitHeight {
        NumberAnimation {
            duration: ControlPanelStyle.animationDuration
            easing.type: Easing.InOutQuad
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            id: headerArea
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            color: headerMouseArea.containsMouse ? ControlPanelStyle.hoverGray : "transparent"
            radius: root.radius

            Behavior on color {
                ColorAnimation { duration: ControlPanelStyle.animationDuration }
            }

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 12

                Label {
                    text: root.expanded ? ControlPanelStyle.iconUp : ControlPanelStyle.iconDropdown
                    font.pixelSize: 10
                    color: ControlPanelStyle.textSecondary
                }

                Label {
                    text: root.structName
                    font.pixelSize: ControlPanelStyle.fontSizeBody
                    font.bold: true
                    color: ControlPanelStyle.textPrimary
                    Layout.fillWidth: true
                }

                Rectangle {
                    Layout.preferredWidth: countLabel.width + 16
                    Layout.preferredHeight: 20
                    color: ControlPanelStyle.primaryBlueLight
                    radius: 10

                    Label {
                        id: countLabel
                        anchors.centerIn: parent
                        text: root.fields.length + " Field" + (root.fields.length !== 1 ? "s" : "")
                        font.pixelSize: ControlPanelStyle.fontSizeMicro
                        font.bold: true
                        color: ControlPanelStyle.primaryBlue
                    }
                }
            }

            MouseArea {
                id: headerMouseArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    root.expanded = !root.expanded;
                    root.isExpandedChanged();
                }
            }
        }

        // Fields Area
        Item {
            id: fieldsArea
            Layout.fillWidth: true
            implicitHeight: fieldsColumn.implicitHeight + 32
            visible: root.expanded
            clip: true

            ColumnLayout {
                id: fieldsColumn
                width: parent.width - 32
                x: 16
                y: 8
                spacing: 12

                Repeater {
                    model: root.fields

                    delegate: StructField {
                        Layout.fillWidth: true
                        fieldName: modelData.name
                        fieldType: modelData.type || "string"
                        fieldValue: root.structObject ? root.structObject[modelData.name] : ""
                        enumModel: modelData.enumModel || null

                        onFieldChanged: function(value) {
                            root.fieldValueChanged(modelData.name, value)
                        }
                    }
                }
            }
        }
    }
}
