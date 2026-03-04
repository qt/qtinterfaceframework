// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype PropertyCard
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-components
    \brief A card container that wraps a single property editor.

    \techpreview

    PropertyCard displays a header with the property name, an optional
    "Non-Zoned" badge, and a content area loaded from the supplied
    \l content component. It provides a hover highlight effect and
    consistent spacing around the editor control.

    \section1 Usage

    \qml
    PropertyCard {
        propertyName: "fanSpeed"
        nonZonedBadge: true
        content: Component {
            SpinControl {
                from: 0
                to: 10
                value: backend.fanSpeed
                onValueModified: function(v) { backend.setFanSpeed(v) }
            }
        }
    }
    \endqml
*/
Rectangle {
    id: root

    /*!
        This property holds the name displayed in the card header.
    */
    property string propertyName: ""

    /*!
        This property holds whether the "Non-Zoned" badge is shown
        in the header. Useful for zoned interfaces where some
        properties are not zone-specific.
    */
    property bool nonZonedBadge: false

    /*!
        This property holds the Component that provides the editor
        control loaded inside the card body.
    */
    property Component content: null

    Layout.fillWidth: true
    Layout.preferredHeight: contentColumn.height + 24
    color: ControlPanelStyle.backgroundWhite
    border.color: ControlPanelStyle.borderLight
    border.width: 1
    radius: 6

    // Hover effect border
    Rectangle {
        id: hoverBorder
        anchors.fill: parent
        color: ControlPanelStyle.primaryBlueLight
        border.width: 0
        radius: parent.radius
        opacity: 0
        Behavior on opacity { NumberAnimation { duration: ControlPanelStyle.animationDuration } }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onEntered: hoverBorder.opacity = 0.4
        onExited: hoverBorder.opacity = 0
    }

    ColumnLayout {
        id: contentColumn
        width: parent.width - 24
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 12
        spacing: ControlPanelStyle.spacingMedium

        // Header Row
        RowLayout {
            Layout.fillWidth: true
            spacing: ControlPanelStyle.spacingMedium

            Label {
                text: root.propertyName
                font.pixelSize: ControlPanelStyle.fontSizeSmall
                font.bold: true
                color: ControlPanelStyle.textPrimary
                Layout.fillWidth: true
            }

            // Non-Zoned Badge only for Zoned interface
            Rectangle {
                visible: root.nonZonedBadge
                Layout.preferredWidth: 70
                Layout.preferredHeight: 16
                color: ControlPanelStyle.badgeNonZoned
                radius: 8
                Label {
                    anchors.centerIn: parent
                    text: "Non-Zoned"
                    font.pixelSize: ControlPanelStyle.fontSizeMicro
                    font.bold: true
                    color: ControlPanelStyle.badgeNonZonedText
                }
            }
        }

        // Content Area
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: contentLoader.item.implicitHeight

            Loader {
                id: contentLoader
                width: parent.width
                height: item ? item.implicitHeight : 32
                sourceComponent: root.content
            }
        }
    }
}
