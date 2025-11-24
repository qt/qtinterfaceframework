// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ComboBoxControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Reusable ComboBox control for both Enums and Domain values.
*/
ComboBox {
    id: root

    property var valueModel: null
    property var value: null
    signal valueActivated(var value)
    property bool isEnumModel : false

    readonly property var selectedValue: {
        if (currentIndex < 0 || !valueModel) return null

        if (isEnumModel) {
            return model.get(currentIndex).value
        } else {
            return model[currentIndex]
        }
    }

    model: valueModel
    textRole: isEnumModel ? "name" : ""
    font.pixelSize: ControlPanelStyle.fontSizeBody
    implicitHeight: ControlPanelStyle.controlHeight

    displayText: {
        if (currentIndex < 0) return ""
        if (isEnumModel) {
            return model.get(currentIndex).name
        } else {
            return String(model[currentIndex])
        }
    }

    Component.onCompleted: updateCurrentIndex()
    onValueChanged: updateCurrentIndex()
    onModelChanged: updateCurrentIndex()

    function updateCurrentIndex() {
        if (!model) return

        if (isEnumModel) {
            for (var i = 0; i < model.count; i++) {
                if (model.get(i).value === value) {
                    currentIndex = i
                    return
                }
            }
        } else {
            currentIndex = model.indexOf(value)
        }
    }

    background: Rectangle {
        color: root.down ? ControlPanelStyle.primaryBlueLight :
               (root.hovered ? ControlPanelStyle.hoverGray : ControlPanelStyle.backgroundGray)
        border.color: ControlPanelStyle.borderLight
        border.width: ControlPanelStyle.borderWidth
        radius: ControlPanelStyle.radius
    }

    contentItem: Label {
        text: root.displayText
        font: root.font
        color: ControlPanelStyle.textPrimary
        verticalAlignment: Text.AlignVCenter
        leftPadding: 12
    }

    onActivated: function(index) {
        if (!model || index < 0) return

        if (isEnumModel) {
            root.valueActivated(model.get(index).value)
        } else {
            root.valueActivated(model[index])
        }
    }
}
