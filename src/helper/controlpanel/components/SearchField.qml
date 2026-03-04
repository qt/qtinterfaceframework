// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype SearchField
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-components
    \brief A styled search input with a magnifying-glass icon and clear
    button.

    \techpreview

    SearchField wraps a TextField with a search icon on the left and a
    clear button that appears when text is entered. It is used by
    \l InterfaceSelector and \l TopBar to filter interfaces and
    properties respectively.

    \section1 Usage

    \qml
    SearchField {
        placeholderText: "Search properties..."
        onTextChanged: propertyList.filterText = text
    }
    \endqml
*/
Rectangle {
    id: root

    /*!
        This property holds the current text in the search input.
    */
    property alias text: searchInput.text

    /*!
        This property holds the placeholder text shown when the input
        is empty.
    */
    property alias placeholderText: searchInput.placeholderText

    implicitHeight: 34
    color: ControlPanelStyle.backgroundWhite
    border.color: searchInput.activeFocus ? ControlPanelStyle.primaryBlue : ControlPanelStyle.borderLight
    border.width: searchInput.activeFocus ? ControlPanelStyle.borderWidthFocus : ControlPanelStyle.borderWidth
    radius: ControlPanelStyle.radius

    Behavior on border.color {
        ColorAnimation { duration: ControlPanelStyle.animationDuration }
    }

    Behavior on border.width {
        NumberAnimation { duration: ControlPanelStyle.animationDuration }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: ControlPanelStyle.spacingSmall
        spacing: ControlPanelStyle.spacingMedium

        // Search Icon
        Label {
            text: ControlPanelStyle.iconSearch
            font.pixelSize: ControlPanelStyle.iconSize
            color: ControlPanelStyle.textSecondary
        }

        // Text Input
        TextField {
            id: searchInput
            Layout.fillWidth: true
            font.pixelSize: ControlPanelStyle.fontSizeBody
            color: ControlPanelStyle.textPrimary

            background: Rectangle {
                color: "transparent"
            }
        }

        // Clear Button
        ToolButton {
            id: clearButton
            visible: searchInput.text.length > 0
            text: ControlPanelStyle.iconClose // Close icon
            font.pixelSize: ControlPanelStyle.iconSize
            Layout.preferredWidth: 20
            Layout.preferredHeight: 20

            background: Rectangle {
                color: clearButton.hovered ? ControlPanelStyle.hoverGray : "transparent"
                radius: 10

                Behavior on color {
                    ColorAnimation { duration: ControlPanelStyle.animationDuration }
                }
            }

            onClicked: {
                searchInput.text = ""
                searchInput.focus = false
            }
        }
    }
}
