// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype PropertySection
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Property Section with header.

    Container for a group of properties with a section header.
    Used to separate "NON-ZONED PROPERTIES" and "ZONED PROPERTIES" sections.
*/
ColumnLayout {
    id: root
    property string title: ""
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
