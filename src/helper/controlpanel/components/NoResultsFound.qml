// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype NoResultsFound
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-components
    \brief A placeholder message shown when a search yields no results.

    \techpreview

    NoResultsFound displays a search icon, a title, and a description
    that includes the search query. It is used by \l InterfaceSelector
    and the property list when the user's filter matches nothing. The
    \l itemType property lets the message adapt to different contexts
    (e.g. "properties", "interfaces").

    \section1 Usage

    \qml
    NoResultsFound {
        searchText: searchField.text
        itemType: "properties"
    }
    \endqml
*/
Rectangle {
    id: root

    /*!
        This property holds the search query to display in the
        description message.
    */
    property string searchText: ""

    /*!
        This property holds the type of items being searched,
        used to build the title and description text
        (e.g. "properties", "interfaces"). Defaults to \c {"properties"}.
    */
    property string itemType: "properties"

    /*!
        This property holds the title text shown in the no-results placeholder
        when a search yields no matches. Defaults to
        \c {"No <itemType> found"}.
    */
    property string titleText: "No " + itemType + " found"

    /*!
        This property holds the description text shown below the
        title. Defaults to \c {'No <itemType> match "<searchText>"'}.
    */
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
