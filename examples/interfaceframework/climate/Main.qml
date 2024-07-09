// Copyright (C) 2024 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Example.If.ClimateModule

Window {

    visible: true
    width: 670
    height: 60
    title: qsTr("Interface Framework Climate Example")
    color: "darkgrey"

    ClimateControl {
        id: climateCtrl
    }

    RowLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        //! [0]
        TempControl {
            value: climateCtrl.zoneAt.FrontLeft.targetTemperature
            onValueModified: {
                climateCtrl.zoneAt.FrontLeft.targetTemperature = value
            }
        }
        //! [0]

        FanSpeedControl {
            value: climateCtrl.zoneAt.FrontLeft.fanSpeedLevel
            onValueModified: {
                climateCtrl.zoneAt.FrontLeft.fanSpeedLevel = value
            }
        }
    }

    RowLayout {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            Layout.leftMargin: 25
            Layout.rightMargin: 5
            text: climateCtrl.outsideTemperature + " °C"
        }

        //! [1]
        Button {
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            Layout.maximumWidth: 50
            text: "A/C"
            checkable: true
            checked: climateCtrl.airConditioning
            onToggled: {
                climateCtrl.airConditioning = checked
            }
        }
        //! [1]

        CheckBox {
            id: recirculationCheckBox
            Layout.rightMargin: 20

            tristate: true
            padding: 0

            indicator: Item {
                implicitWidth: 40
                implicitHeight: 40
                y: parent.topPadding + (parent.availableHeight - height) / 2

                IconImage {
                    anchors.centerIn: parent
                    source: recirculationCheckBox.checkState === Qt.Checked ? "assets/air_circulation.png" :
                            recirculationCheckBox.checkState === Qt.PartiallyChecked ? "assets/air_circulation_auto.png"
                                                                                     : "assets/air_circulation_off.png"
                }
            }

            checkState: climateCtrl.recirculationMode === ClimateModule.RecirculationOn ? Qt.Checked :
                        climateCtrl.recirculationMode === ClimateModule.AutoRecirculation ? Qt.PartiallyChecked : Qt.Unchecked

            onCheckStateChanged: {
                let mode = ClimateModule.RecirculationOff
                if (checkState === Qt.Checked)
                    mode = ClimateModule.RecirculationOn
                else if (checkState === Qt.PartiallyChecked)
                    mode = ClimateModule.AutoRecirculation

                climateCtrl.recirculationMode = mode
            }
        }
    }

    RowLayout {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        FanSpeedControl {
            value: climateCtrl.zoneAt.FrontRight.fanSpeedLevel
            onValueModified: {
                climateCtrl.zoneAt.FrontRight.fanSpeedLevel = value
            }
        }

        TempControl {
            value: climateCtrl.zoneAt.FrontRight.targetTemperature
            onValueModified: {
                climateCtrl.zoneAt.FrontRight.targetTemperature = value
            }
        }
    }
}
