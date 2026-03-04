// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype TopBar
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-panels
    \brief A toolbar displayed above the property list in the control panel.

    \techpreview

    TopBar shows the selected interface name, a \l ZoneSelector drop-down
    (for zoned interfaces), a \l SearchField for filtering properties,
    and filter buttons that toggle between all, non-zoned, and zoned
    properties. Filter buttons and the zone selector are only visible
    when \l isZoned is \c true.

    \section1 Usage

    \qml
    TopBar {
        interfaceName: "ClimateControl"
        isZoned: true
        availableZones: ["FrontLeft", "FrontRight", "Rear"]
        currentZoneIndex: 0
        onZoneChanged: function(index) {
            controlPanel.selectZone(index)
        }
    }
    \endqml
*/
Rectangle {
    id: root

    /*!
        This property holds the name of the currently selected
        interface, displayed in the toolbar header.
    */
    property string interfaceName: ""

    /*!
        This property holds whether the current interface supports
        zones. When \c true, the zone selector and filter buttons
        are shown.
    */
    property bool isZoned: false

    /*!
        This property holds the list of available zone names for the
        \l ZoneSelector drop-down.
    */
    property var availableZones: []

    /*!
        This property holds the index of the currently selected zone
        in \l availableZones.
    */
    property int currentZoneIndex: 0

    /*!
        This property holds the current text in the property search
        field, used to filter the property list below.
    */
    property string propertySearchText: ""

    /*!
        This property holds the active property filter index.
        \c 0 means all properties, \c 1 means non-zoned only, and
        \c 2 means zoned only.
    */
    property int propertyFilter: 0

    /*!
        \qmlsignal TopBar::zoneChanged(int index)

        This signal is emitted when the user selects a different zone.
        The \a index parameter is the position in \l availableZones.
    */
    signal zoneChanged(int index)

    implicitHeight: 60
    color: ControlPanelStyle.backgroundWhite

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: ControlPanelStyle.borderLight
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 16

        // Interface Icon
        Rectangle {
            Layout.preferredWidth: 32
            Layout.preferredHeight: 32
            color: ControlPanelStyle.primaryBlueLight
            radius: 6

            Label {
                anchors.centerIn: parent
                text: ControlPanelStyle.iconGear  // Gear icon
                font.pixelSize: 18
                color: ControlPanelStyle.primaryBlue
            }
        }

        // Interface Name and Description
        ColumnLayout {
            spacing: 2

            Label {
                text: root.interfaceName
                font.pixelSize: ControlPanelStyle.fontSizeH2
                font.bold: true
                color: ControlPanelStyle.textPrimary
            }

            Label {
                text: "Interface Configuration"
                font.pixelSize: ControlPanelStyle.fontSizeCaption
                color: ControlPanelStyle.textSecondary
            }
        }

        // Separator (only if zoned)
        Rectangle {
            visible: root.isZoned
            Layout.preferredWidth: 1
            Layout.preferredHeight: 24
            color: ControlPanelStyle.borderLight
            Layout.leftMargin: 8
            Layout.rightMargin: 8
        }

        // Zone Label
        Label {
            visible: root.isZoned
            text: "Zone:"
            font.pixelSize: ControlPanelStyle.fontSizeSmall
            color: ControlPanelStyle.textSecondary
        }

        // Zone Selector
        ZoneSelector {
            visible: root.isZoned
            zones: root.availableZones
            currentZoneIndex: root.currentZoneIndex

            onZoneSelected: function(index) {
                root.zoneChanged(index)
            }
        }

        // Spacer
        Item { Layout.fillWidth: true }

        // Property Search Field
        SearchField {
            Layout.preferredWidth: 240
            placeholderText: "Search properties..."
            text: root.propertySearchText

            onTextChanged: {
                root.propertySearchText = text
            }
        }

        // Separator (only if zoned)
        Rectangle {
            visible: root.isZoned
            Layout.preferredWidth: 1
            Layout.preferredHeight: 24
            color: ControlPanelStyle.borderLight
            Layout.leftMargin: 8
            Layout.rightMargin: 8
        }

        // Filter Buttons (only if zoned)
        Repeater {
            model: root.isZoned ? [
                { icon: ControlPanelStyle.iconAllProperties, text: "All" },
                { icon: ControlPanelStyle.iconNonZoned, text: "Non-Zoned" },
                { icon: ControlPanelStyle.iconZoned, text: "Zoned" }
            ] : []

            delegate: Rectangle {
                id: filterDelegate
                required property int index
                required property var modelData

                Layout.preferredWidth: 44
                Layout.preferredHeight: 44
                color: root.propertyFilter === index ? ControlPanelStyle.primaryBlueLight : "transparent"
                radius: ControlPanelStyle.radius

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 2

                    Label {
                        text: filterDelegate.modelData.icon
                        font.pixelSize: ControlPanelStyle.fontSizeH1
                        color: root.propertyFilter === filterDelegate.index ?
                               ControlPanelStyle.primaryBlue : ControlPanelStyle.textSecondary
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Label {
                        visible: root.propertyFilter === filterDelegate.index
                        text: filterDelegate.modelData.text
                        font.pixelSize: ControlPanelStyle.fontSizeMicro
                        color: ControlPanelStyle.primaryBlue
                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onEntered: {
                        if (root.propertyFilter !== filterDelegate.index) {
                            parent.color = ControlPanelStyle.hoverGray
                        }
                    }

                    onExited: {
                        if (root.propertyFilter !== filterDelegate.index) {
                            parent.color = "transparent"
                        }
                    }

                    onClicked: {
                        root.propertyFilter = filterDelegate.index
                    }
                }

                Behavior on color {
                    ColorAnimation { duration: ControlPanelStyle.animationDuration }
                }
            }
        }

        // More Options Button
        ToolButton {
            id: moreOptions
            text: ControlPanelStyle.iconMenu   // Vertical ellipsis
            font.pixelSize: 16

            ToolTip.visible: hovered
            ToolTip.text: "More options"

            background: Rectangle {
                color: moreOptions.down ? ControlPanelStyle.primaryBlueLight :
                       (moreOptions.hovered ? ControlPanelStyle.hoverGray : "transparent")
                radius: ControlPanelStyle.radius
            }

            onClicked: console.log("More features coming soon..")
        }
    }
}
