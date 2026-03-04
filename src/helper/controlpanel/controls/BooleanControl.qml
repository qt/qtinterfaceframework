// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype BooleanControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-controls
    \brief A toggle switch control for editing boolean properties.

    \techpreview

    BooleanControl provides a styled Switch with an accompanying status label
    that displays "Enabled" or "Disabled" based on the current state. It is
    used by the simulation control panel for all boolean-typed properties.

    \section1 Usage

    \qml
    BooleanControl {
        checked: backend.headlightsOn
        onToggled: function(checked) {
            backend.setHeadlightsOn(checked)
        }
    }
    \endqml
*/

RowLayout {
    id: root

    /*!
        This property holds whether the switch is checked.

        By default, the property is set to \c false.
    */
    property bool checked: false

    /*!
        \qmlsignal BooleanControl::toggled(bool checked)

        This signal is emitted when the user toggles the switch.
        The \a checked parameter contains the new state.
    */
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
