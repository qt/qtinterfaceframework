// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype InterfaceSelector
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Interface Selector Panel.
*/

Rectangle {
    id: root
    property var interfaces: []
    property int currentInterfaceIndex: 0
    property string searchText: ""
    signal interfaceSelected(int index)

    readonly property var filteredInterfaces: {
        if (!searchText || searchText.length === 0) {
            return interfaces
        }
        var filtered = []
        for (var i = 0; i < interfaces.length; i++) {
            if (interfaces[i].name.toLowerCase().indexOf(searchText.toLowerCase()) !== -1) {
                filtered.push(interfaces[i])
            }
        }
        return filtered
    }

    readonly property bool currentIsZoned: interfaces.length > 0 && currentInterfaceIndex >= 0 ?
                                            interfaces[currentInterfaceIndex].isZoned : false
    color: ControlPanelStyle.backgroundGray

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 48
            color: ControlPanelStyle.primaryBlue

            Label {
                anchors.centerIn: parent
                text: "INTERFACES"
                font.pixelSize: ControlPanelStyle.fontSizeSmall
                font.bold: true
                font.letterSpacing: 1
                color: ControlPanelStyle.backgroundWhite
            }
        }

        SearchField {
            Layout.fillWidth: true
            Layout.margins: 12
            Layout.topMargin: 16
            placeholderText: "Search interfaces..."
            text: root.searchText

            onTextChanged: {
                root.searchText = text
            }
        }

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ListView {
                id: interfaceListView
                width: parent.width
                model: root.filteredInterfaces
                spacing: 2

                delegate: Rectangle {
                    id: interfaceDelegate
                    required property int index
                    required property var modelData

                    width: interfaceListView.width
                    height: 50

                    property int actualIndex: {
                        for (var i = 0; i < root.interfaces.length; i++) {
                            if (root.interfaces[i].name === modelData.name) {
                                return i
                            }
                        }
                        return -1
                    }

                    color: root.currentInterfaceIndex === actualIndex ?
                           ControlPanelStyle.primaryBlueLight : "transparent"

                    Rectangle {
                        anchors.left: parent.left
                        width: 3
                        height: parent.height
                        color: root.currentInterfaceIndex === interfaceDelegate.actualIndex ?
                               ControlPanelStyle.primaryBlue : "transparent"
                        Behavior on color { ColorAnimation { duration: ControlPanelStyle.animationDuration } }
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 16
                        anchors.rightMargin: 16
                        spacing: 10

                        Label {
                            text: interfaceDelegate.modelData.isZoned ? ControlPanelStyle.iconZoned : ControlPanelStyle.iconNonZoned
                            font.pixelSize: 16
                            color: root.currentInterfaceIndex === interfaceDelegate.actualIndex ?
                                   ControlPanelStyle.primaryBlue : ControlPanelStyle.textSecondary
                        }

                        Label {
                            text: interfaceDelegate.modelData.name
                            font.pixelSize: ControlPanelStyle.fontSizeBody
                            color: root.currentInterfaceIndex === interfaceDelegate.actualIndex ?
                                   ControlPanelStyle.primaryBlue : ControlPanelStyle.textPrimary
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onEntered: {
                            if (root.currentInterfaceIndex !== interfaceDelegate.actualIndex) {
                                parent.color = ControlPanelStyle.hoverGray
                            }
                        }
                        onExited: {
                            if (root.currentInterfaceIndex !== interfaceDelegate.actualIndex) {
                                parent.color = "transparent"
                            }
                        }
                        onClicked: {
                            root.interfaceSelected(interfaceDelegate.actualIndex)
                        }
                    }
                    Behavior on color { ColorAnimation { duration: ControlPanelStyle.animationDuration } }
                }

                // No results footer
                footer: Item {
                    width: interfaceListView.width
                    height: visible ? 150 : 0
                    visible: root.searchText.length > 0 && root.filteredInterfaces.length === 0

                    NoResultsFound {
                        anchors.centerIn: parent
                        width: parent.width
                        searchText: root.searchText
                        itemType: "interfaces"
                    }
                }
            }
        }

        // Info panel
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: ControlPanelStyle.backgroundDarkGray

            Rectangle {
                anchors.top: parent.top
                width: parent.width
                height: 1
                color: ControlPanelStyle.borderLight
            }

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 8

                Label {
                    text: root.filteredInterfaces.length + " Interface" + (root.filteredInterfaces.length !== 1 ? "s" : "")
                    font.pixelSize: ControlPanelStyle.fontSizeSmall
                    font.bold: true
                    color: ControlPanelStyle.textPrimary
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle {
                    Layout.preferredWidth: statusLabel.width + 16
                    Layout.preferredHeight: 20
                    Layout.alignment: Qt.AlignHCenter
                    color: root.currentIsZoned ? ControlPanelStyle.badgeZoned : ControlPanelStyle.badgeNonZoned
                    radius: 10

                    Label {
                        id: statusLabel
                        anchors.centerIn: parent
                        text: root.currentIsZoned ? "Zoned" : "Non-Zoned"
                        font.pixelSize: ControlPanelStyle.fontSizeTiny
                        font.bold: true
                        color: root.currentIsZoned ? ControlPanelStyle.badgeZonedText : ControlPanelStyle.badgeNonZonedText
                    }
                }
            }
        }
    }
}
