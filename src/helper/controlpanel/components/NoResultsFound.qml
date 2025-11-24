// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype NoResultsFound
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief No Results Found Message.

    Displays a "no results found" message when search returns no results.
    Can be used for properties, interfaces, or any searchable list.
 */
Rectangle {
    id: root
    property string searchText: ""
    property string itemType: "properties"
    property string titleText: "No " + itemType + " found"
    property string descriptionText: 'No ' + itemType + ' match "' + searchText + '"'

    implicitHeight: 120
    color: "transparent"

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 12

        Label {
            text: ControlPanelStyle.iconSearch
            font.pixelSize: ControlPanelStyle.iconSizeLarge
            Layout.alignment: Qt.AlignHCenter
            opacity: 0.5
        }

        Label {
            text: root.titleText
            font.pixelSize: ControlPanelStyle.fontSizeH1
            font.bold: true
            color: ControlPanelStyle.textPrimary
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: root.descriptionText
            font.pixelSize: ControlPanelStyle.fontSizeSmall
            color: ControlPanelStyle.textSecondary
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
