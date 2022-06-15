// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import QtInterfaceFramework.VehicleFunctions

ApplicationWindow {
    title: "Climate Control"
    width: 384
    height: 324
    visible: true

    //![1]
    ClimateControl {
        id: climateControl
        discoveryMode: ClimateControl.LoadOnlySimulationBackends
    }
    //![1]

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        //![2]
        GroupBox {
            title: "Air Flow Direction"

            ColumnLayout {
                anchors.fill: parent
                CheckBox {
                    text: "Windshield"
                    checked: climateControl.airflowDirections & ClimateControl.Windshield
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirections |= ClimateControl.Windshield
                        else
                            climateControl.airflowDirections &= ~ClimateControl.Windshield
                    }
                }

                CheckBox {
                    text: "Dashboard"
                    checked: climateControl.airflowDirections & ClimateControl.Dashboard
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirections |= ClimateControl.Dashboard
                        else
                            climateControl.airflowDirections &= ~ClimateControl.Dashboard
                    }
                }

                CheckBox {
                    text: "Floor"
                    checked: climateControl.airflowDirections & ClimateControl.Floor
                    onClicked: {
                        if (checked)
                            climateControl.airflowDirections |= ClimateControl.Floor
                        else
                            climateControl.airflowDirections &= ~ClimateControl.Floor
                    }
                }
            }
        }

        CheckBox {
            text: "Air Condition"
            checked: climateControl.airConditioningEnabled
            onClicked: {
                    climateControl.airConditioningEnabled = checked
            }
        }

        CheckBox {
            text: "Heater"
            checked: climateControl.heaterEnabled
            onClicked: {
                    climateControl.heaterEnabled = checked
            }
        }

        CheckBox {
            text: "Air Recirculation"
            checked: climateControl.recirculationMode === ClimateControl.RecirculationOn
            onClicked: {
                if (checked)
                    climateControl.recirculationMode = ClimateControl.RecirculationOn
                else
                    climateControl.recirculationMode = ClimateControl.RecirculationOff
            }
        }

        ColumnLayout {
            RowLayout {

                Label {
                    text: "Fan Speed"
                }

                SpinBox {
                    value: climateControl.fanSpeedLevel
                    onValueChanged: {
                        climateControl.fanSpeedLevel = value
                    }
                }
            }


            RowLayout {

                Label {
                    text: "Steering Wheel Heater"
                }

                SpinBox {
                    value: climateControl.steeringWheelHeater
                    onValueChanged: {
                        climateControl.steeringWheelHeater = value
                    }
                }
            }
        }
        //![2]
        Row {
            //![3]
            GroupBox {
                title: "Front Left Zone"

                ColumnLayout {
                    RowLayout {

                        Label {
                            text: "Temperature"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontLeft.targetTemperature
                            onValueChanged: {
                                climateControl.zoneAt.FrontLeft.targetTemperature = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontLeft.seatHeater
                            onValueChanged: {
                                climateControl.zoneAt.FrontLeft.seatHeater = value
                            }
                        }
                    }
                }
            }

            GroupBox {
                title: "Front Right Zone"

                ColumnLayout {
                    RowLayout {

                        Label {
                            text: "Temperature"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontRight.targetTemperature
                            onValueChanged: {
                                climateControl.zoneAt.FrontRight.targetTemperature = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.FrontRight.seatHeater
                            onValueChanged: {
                                climateControl.zoneAt.FrontRight.seatHeater = value
                            }
                        }
                    }
                }
            }

            GroupBox {
                title: "Rear Zone"

                ColumnLayout {
                    RowLayout {

                        Label {
                            text: "Temperature"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.Rear.targetTemperature
                            onValueChanged: {
                                climateControl.zoneAt.Rear.targetTemperature = value
                            }
                        }
                    }


                    RowLayout {

                        Label {
                            text: "Seat Heater"
                        }

                        SpinBox {
                            value: climateControl.zoneAt.Rear.seatHeater
                            onValueChanged: {
                                climateControl.zoneAt.Rear.seatHeater = value
                            }
                        }
                    }
                }
            }
            //![3]
        }
    }

    Dialog {
        id: messageDialog
        anchors.centerIn: parent
        title: "Auto Discovery Failed !"
        standardButtons: Dialog.Ok

        Label {
            text: "No Climate Backend available"
        }
    }

    //![4]
    Component.onCompleted: {
        if (!climateControl.isValid)
            messageDialog.open()
    }
    //![4]
}
