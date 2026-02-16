// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtInterfaceFramework.ControlPanelHelper

/*!
    \qmltype TirePressureControl
    \brief A custom control panel editor for the TirePressure struct.

    TirePressureControl is an example of a \l CustomControlPanelComponent
    that replaces the default \l StructControl for the \c tirePressure
    property.

    Values are displayed in bar with one decimal place. Changing any
    SpinBox writes the updated struct back to the simulation backend via
    \l {CustomControlPanelComponent::setValue()}{setValue()}.
*/
CustomControlPanelComponent {
    id: root

    implicitHeight: outerColumn.implicitHeight

    function pressureColor(pressure) {
        if (pressure < 1.5) return "#e53935"   // red   – critically low
        if (pressure < 2.0) return "#fb8c00"   // amber – slightly low
        if (pressure <= 3.0) return "#43a047"  // green – nominal
        return "#e53935"                        // red   – over-inflated
    }

    // Draws a single arc gauge onto canvas for the given pressure.
    function drawGauge(canvas, pressure) {
        var ctx = canvas.getContext("2d")
        ctx.clearRect(0, 0, canvas.width, canvas.height)

        var cx = canvas.width / 2
        var cy = canvas.height * 0.54
        var r  = canvas.width * 0.34
        var lw = canvas.width * 0.09

        // Arc spans 270° starting from the 8-o'clock position (135° from +x axis)
        var startAngle = Math.PI * 0.75
        var totalAngle = Math.PI * 1.5

        // Background track
        ctx.beginPath()
        ctx.arc(cx, cy, r, startAngle, startAngle + totalAngle, false)
        ctx.strokeStyle = "#d4d4d4"
        ctx.lineWidth  = lw
        ctx.lineCap    = "round"
        ctx.stroke()

        // Colored value arc
        var fraction = Math.max(0, Math.min(1, pressure / 5.0))
        if (fraction > 0) {
            ctx.beginPath()
            ctx.arc(cx, cy, r, startAngle, startAngle + fraction * totalAngle, false)
            ctx.strokeStyle = pressureColor(pressure)
            ctx.lineWidth   = lw
            ctx.lineCap     = "round"
            ctx.stroke()
        }

        // Value text
        var fontSize = Math.round(canvas.width * 0.18)
        ctx.fillStyle    = canvas.textColor
        ctx.font         = "bold " + fontSize + "px system-ui, sans-serif"
        ctx.textAlign    = "center"
        ctx.textBaseline = "middle"
        ctx.fillText(pressure.toFixed(1), cx, cy - fontSize * 0.28)

        // Unit label
        var unitSize = Math.round(canvas.width * 0.11)
        ctx.fillStyle = canvas.dimColor
        ctx.font      = unitSize + "px system-ui, sans-serif"
        ctx.fillText("bar", cx, cy + fontSize * 0.52)
    }

    //! [snippet]
    ColumnLayout {
        id: outerColumn
        width: parent.width
        spacing: 0

        Label {
            text: qsTr("Tire Pressure")
            font.bold: true
            font.pixelSize: Qt.application.font.pixelSize * 1.1
            color: ControlPanelStyle.textSecondary
            Layout.bottomMargin: ControlPanelStyle.spacingLarge
        }

        GridLayout {
            id: grid
            Layout.fillWidth: true
            columns: 4
            columnSpacing: ControlPanelStyle.spacingLarge * 3
            rowSpacing: ControlPanelStyle.spacingLarge

            Repeater {
                model: [
                    { key: "frontLeft",  label: qsTr("Front Left")  },
                    { key: "frontRight", label: qsTr("Front Right") },
                    { key: "rearLeft",   label: qsTr("Rear Left")   },
                    { key: "rearRight",  label: qsTr("Rear Right")  }
                ]

                ColumnLayout {
                    required property var modelData
                    spacing: 20

                    Canvas {
                        id: gaugeCanvas
                        Layout.alignment: Qt.AlignHCenter
                        width: 90; height: 78

                        property real pressure: root.backendProperty
                                                ? (root.backendProperty[modelData.key] ?? 0)
                                                : 0
                        property color textColor: ControlPanelStyle.textSecondary
                        property color dimColor:  Qt.rgba(
                                                      ControlPanelStyle.textSecondary.r,
                                                      ControlPanelStyle.textSecondary.g,
                                                      ControlPanelStyle.textSecondary.b, 0.6)

                        onPressureChanged: requestPaint()
                        onPaint: root.drawGauge(gaugeCanvas, pressure)
                    }

                    Label {
                        text: modelData.label
                        Layout.alignment: Qt.AlignHCenter
                        color: ControlPanelStyle.textSecondary
                        font.pixelSize: Qt.application.font.pixelSize * 0.85
                    }

                    SpinBox {
                        id: wheelSpinBox
                        Layout.alignment: Qt.AlignHCenter
                        from: 0; to: 50
                        value: root.backendProperty
                               ? Math.round((root.backendProperty[modelData.key] ?? 0) * 10)
                               : 0
                        textFromValue: function(v) { return (v / 10).toFixed(1) + " bar" }
                        valueFromText: function(t) { return Math.round(parseFloat(t) * 10) }
                        onValueModified: {
                            var updated = {
                                frontLeft:  root.backendProperty ? root.backendProperty.frontLeft  : 0,
                                frontRight: root.backendProperty ? root.backendProperty.frontRight : 0,
                                rearLeft:   root.backendProperty ? root.backendProperty.rearLeft   : 0,
                                rearRight:  root.backendProperty ? root.backendProperty.rearRight  : 0
                            }
                            updated[modelData.key] = value / 10
                            root.setValue(updated)
                        }
                    }
                }
            }
        }
    }
    //! [snippet]
}
