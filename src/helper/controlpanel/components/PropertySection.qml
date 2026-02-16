// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype PropertySection
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-components
    \brief A titled group container for \l {PropertyCard}{PropertyCards}.

    PropertySection provides a section header label followed by its
    child items. It is used to visually separate "NON-ZONED PROPERTIES"
    and "ZONED PROPERTIES" groups inside the control panel. The header
    is hidden when \l title is empty.

    \section1 Usage

    \qml
    PropertySection {
        title: "ZONED PROPERTIES"

        PropertyCard { propertyName: "fanSpeed"; ... }
        PropertyCard { propertyName: "targetTemperature"; ... }
    }
    \endqml
*/
ColumnLayout {
    id: root

    /*!
        This property holds the section header text. When empty, the
        header label is hidden.
    */
    property string title: ""

    /*!
        This property holds whether the header label is visible.
        Defaults to \c true when \l title is not empty.
    */
    property bool showTitle: title.length > 0
    property int propertySpacing: 12
    Layout.fillWidth: true
    spacing: propertySpacing

    // Section Header
    Label {
        visible: root.showTitle
        text: root.title
        font.pixelSize: 10
        font.bold: true
        font.letterSpacing: 0.5
        color: ControlPanelStyle.textSecondary
        leftPadding: 2
    }

    default property alias content: root.children
}
