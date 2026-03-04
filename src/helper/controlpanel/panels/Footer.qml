// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype Footer
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-panels
    \brief A status bar displayed at the bottom of the control panel.

    \techpreview

    Footer shows connection status with an animated indicator, the
    current interface name, the active zone (when applicable), and
    the module name. It gives the user an at-a-glance overview of
    the simulation context.

    \section1 Usage

    \qml
    Footer {
        connected: backend.isConnected
        interfaceName: "ClimateControl"
        isZoned: true
        currentZone: "FrontLeft"
        moduleName: "QtIfVehicleFunctions"
    }
    \endqml
*/
Rectangle {
    id: root

    /*!
        This property holds whether the backend is connected.
        Controls the color and animation of the status indicator.
    */
    property bool connected: true

    /*!
        This property holds the name of the currently selected
        interface.
    */
    property string interfaceName: ""

    /*!
        This property holds whether the current interface supports
        zones. When \c true, the \l currentZone label is shown.
    */
    property bool isZoned: false

    /*!
        This property holds the name of the currently active zone.
    */
    property string currentZone: ""

    /*!
        This property holds the module name displayed at the right
        side of the footer.
    */
    property string moduleName: ""
    implicitHeight: 52
    color: ControlPanelStyle.backgroundWhite

    // Top border
    Rectangle {
        anchors.top: parent.top
        width: parent.width
        height: 1
        color: ControlPanelStyle.borderLight
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 12

        // Connection Status Indicator
        Rectangle {
            Layout.preferredWidth: 8
            Layout.preferredHeight: 8
            radius: 4
            color: root.connected ? ControlPanelStyle.accentGreen : ControlPanelStyle.textSecondary

            SequentialAnimation on opacity {
                running: root.connected
                loops: Animation.Infinite
                NumberAnimation {
                    from: 1.0
                    to: 0.4
                    duration: 1000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    from: 0.4
                    to: 1.0
                    duration: 1000
                    easing.type: Easing.InOutQuad
                }
            }
        }

        Label {
            text: root.connected ? "Connected" : "Disconnected"
            font.pixelSize: ControlPanelStyle.fontSizeSmall
            font.bold: true
            color: root.connected ? ControlPanelStyle.accentGreen : ControlPanelStyle.textSecondary
        }

        Rectangle {
            Layout.preferredWidth: 1
            Layout.preferredHeight: 20
            color: ControlPanelStyle.borderLight
            Layout.leftMargin: 4
            Layout.rightMargin: 4
        }

        Label {
            text: "Interface: " + root.interfaceName
            font.pixelSize: ControlPanelStyle.fontSizeCaption
            color: ControlPanelStyle.textSecondary
        }

        // Separator (only if zoned)
        Rectangle {
            visible: root.isZoned
            Layout.preferredWidth: 1
            Layout.preferredHeight: 20
            color: ControlPanelStyle.borderLight
            Layout.leftMargin: 4
            Layout.rightMargin: 4
        }

        // Current Zone
        Label {
            visible: root.isZoned
            text: "Zone: " + root.currentZone
            font.pixelSize: ControlPanelStyle.fontSizeCaption
            color: ControlPanelStyle.textSecondary
        }

        // Spacer
        Item { Layout.fillWidth: true }

        Label {
            text: "Module: " + root.moduleName
            font.pixelSize: ControlPanelStyle.fontSizeCaption
            color: ControlPanelStyle.textSecondary
        }

        Rectangle {
            Layout.preferredWidth: 1
            Layout.preferredHeight: 20
            color: ControlPanelStyle.borderLight
            Layout.leftMargin: 4
            Layout.rightMargin: 4
        }

        Label {
            text: "Qt IF Framework"
            font.pixelSize: ControlPanelStyle.fontSizeCaption
            font.bold: true
            color: ControlPanelStyle.primaryBlue
        }
    }
}
