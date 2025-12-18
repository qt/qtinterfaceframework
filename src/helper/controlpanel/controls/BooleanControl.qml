// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype BooleanControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Reusable Boolean control with Switch.
*/

RowLayout {
    id: root
    property bool checked: false
    signal toggled(bool checked)
    spacing: ControlPanelStyle.spacingMedium

    Switch {
        id: switchControl
        checked: root.checked
        onToggled: {
            root.toggled(checked)
        }
    }

    Label {
        text: switchControl.checked ? "Enabled" : "Disabled"
        font.pixelSize: ControlPanelStyle.fontSizeSmall
        color: ControlPanelStyle.textSecondary
    }

    Item { Layout.fillWidth: true }
}
