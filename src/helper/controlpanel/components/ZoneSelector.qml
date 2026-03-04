// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ZoneSelector
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-components
    \brief A drop-down for selecting the active zone in a zoned interface.

    \techpreview

    ZoneSelector is a styled ComboBox that lists the available zones
    and emits \l zoneSelected when the user picks a different one. It is
    used by \l TopBar to switch between zones of a zoned interface.

    \section1 Usage

    \qml
    ZoneSelector {
        zones: ["FrontLeft", "FrontRight", "Rear"]
        onZoneSelected: function(index) {
            controlPanel.selectZone(index)
        }
    }
    \endqml
*/
ComboBox {
    id: root

    /*!
        This property holds the list of zone names displayed in the
        drop-down.
    */
    property var zones: []

    /*!
        This property holds the index of the currently selected zone.
    */
    property alias currentZoneIndex: root.currentIndex

    /*!
        \qmlsignal ZoneSelector::zoneSelected(int index)

        This signal is emitted when the user selects a zone.
        The \a index parameter is the position in \l zones.
    */
    signal zoneSelected(int index)

    model: zones
    font.pixelSize: ControlPanelStyle.fontSizeBody
    font.bold: true
    implicitHeight: 34
    implicitWidth: 160

    onActivated: function(index) {
        root.zoneSelected(index)
    }

    background: Rectangle {
        color: root.down ? ControlPanelStyle.primaryBlueLight :
               (root.hovered ? ControlPanelStyle.hoverGray : ControlPanelStyle.backgroundGray)
        border.color: root.activeFocus ? ControlPanelStyle.primaryBlue : ControlPanelStyle.borderLight
        border.width: root.activeFocus ? ControlPanelStyle.borderWidthFocus : ControlPanelStyle.borderWidth
        radius: ControlPanelStyle.radius

        Behavior on color {
            ColorAnimation { duration: ControlPanelStyle.animationDuration }
        }

        Behavior on border.color {
            ColorAnimation { duration: ControlPanelStyle.animationDuration }
        }
    }

    contentItem: Label {
        text: root.displayText
        font: root.font
        color: ControlPanelStyle.textPrimary
        verticalAlignment: Text.AlignVCenter
        leftPadding: ControlPanelStyle.spacingLarge
    }

    indicator: Label {
        x: root.width - width - ControlPanelStyle.spacingMedium
        y: root.topPadding + (root.availableHeight - height) / 2
        text: ControlPanelStyle.iconDropdown  // Dropdown indicator
        font.pixelSize: ControlPanelStyle.iconSizeMini
        color: ControlPanelStyle.textSecondary
    }
}
