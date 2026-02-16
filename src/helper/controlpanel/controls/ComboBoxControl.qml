// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype ComboBoxControl
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-controls
    \brief A drop-down selector for editing enum and domain-value properties.

    ComboBoxControl provides a styled ComboBox that can operate in two modes:
    enum mode and domain mode. In enum mode (\l isEnumModel set to \c true),
    it expects a ListModel whose elements have \c name and \c value roles.
    In domain mode it accepts a plain JavaScript array of values.

    The control automatically synchronises the selected index when the
    \l value property changes externally, and emits \l valueActivated
    when the user picks a new entry.

    \section1 Usage

    \section2 With an enum model
    \qml
    ComboBoxControl {
        isEnumModel: true
        valueModel: climateModule.airflowDirectionsModel
        value: backend.airflowDirection
        onValueActivated: function(val) {
            backend.setAirflowDirection(val)
        }
    }
    \endqml

    \section2 With a domain list
    \qml
    ComboBoxControl {
        valueModel: ["Economy", "Comfort", "Sport"]
        value: backend.driveMode
        onValueActivated: function(val) {
            backend.setDriveMode(val)
        }
    }
    \endqml
*/
ComboBox {
    id: root

    /*!
        This property holds the model that provides the selectable values.

        When \l isEnumModel is \c true, this must be a ListModel with
        \c name and \c value roles. Otherwise it should be a plain
        JavaScript array.

        By default, the property is set to \c null.
    */
    property var valueModel: null

    /*!
        This property holds the currently selected value. Setting this
        property programmatically updates the selected index to match.

        By default, the property is set to \c null.
    */
    property var value: null

    /*!
        \qmlsignal ComboBoxControl::valueActivated(var value)

        This signal is emitted when the user selects a new entry from the
        drop-down. The \a value parameter contains the selected value.
    */
    signal valueActivated(var value)

    /*!
        This property controls whether the \l valueModel is treated as an
        enum ListModel (with \c name and \c value roles) or as a plain
        JavaScript array.

        By default, the property is set to \c false.
    */
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
