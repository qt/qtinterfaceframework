// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Layouts

import QtInterfaceFramework.VehicleFunctions

GroupBox {
    property WindowControl zone

    function stateToString(state) {
        if (state === WindowControl.FullyOpen)
            return "Fully Open";
        else if (state === WindowControl.Open)
            return "Open";
        else
            return "Closed";
    }

    ColumnLayout {
        RowLayout {

            Label {
                text: "Window state: "
            }

            Label {
                text: stateToString(zone.state)
            }
        }

        Button {
            text: "open"
            onClicked: zone.open().then(function(){console.error(zone.zone + " window opened")}, function(){console.error("opening " + zone.zone + " window failed!")})
        }

        Button {
            text: "close"
            onClicked: zone.close().then(function(){console.error(zone.zone + " window closed")}, function(){console.error("closing " + zone.zone + " window failed!")})
        }

        RowLayout {
            GroupBox {
                title: "Window blind"

                ColumnLayout {
                    RowLayout {

                        Label {
                            text: "State: "
                        }

                        Label {
                            text: stateToString(zone.blindState)
                        }
                    }

                    ButtonGroup {
                        id: blindGroup
                    }

                    RadioButton {
                        text: "Open"
                        ButtonGroup.group: blindGroup
                        checked: zone.blindMode === WindowControl.BlindOpen
                        onCheckedChanged: {
                            if (checked)
                                zone.blindMode = WindowControl.BlindOpen
                        }
                    }

                    RadioButton {
                        text: "Closed"
                        ButtonGroup.group: blindGroup
                        checked: zone.blindMode === WindowControl.BlindClosed
                        onCheckedChanged: {
                            if (checked)
                                zone.blindMode = WindowControl.BlindClosed
                        }
                    }

                    RadioButton {
                        text: "Automatic"
                        ButtonGroup.group: blindGroup
                        checked: zone.blindMode === WindowControl.AutoBlind
                        onCheckedChanged: {
                            if (checked)
                                zone.blindMode = WindowControl.AutoBlind
                        }
                    }
                }
            }

            GroupBox {
                title: "Heater"

                ColumnLayout {

                    RowLayout {

                        Label {
                            text: "Running: "
                        }

                        Label {
                            text: zone.heater
                        }
                    }

                    ButtonGroup {
                        id: heaterGroup
                    }

                    RadioButton {
                        text: "On"
                        ButtonGroup.group: heaterGroup
                        checked: zone.heaterMode === WindowControl.HeaterOn
                        onCheckedChanged: {
                            if (checked)
                                zone.heaterMode = WindowControl.HeaterOn
                        }
                    }

                    RadioButton {
                        text: "Off"
                        ButtonGroup.group: heaterGroup
                        checked: zone.heaterMode === WindowControl.HeaterOff
                        onCheckedChanged: {
                            if (checked)
                                zone.heaterMode = WindowControl.HeaterOff
                        }
                    }

                    RadioButton {
                        text: "Automatic"
                        ButtonGroup.group: heaterGroup
                        checked: zone.heaterMode === WindowControl.AutoHeater
                        onCheckedChanged: {
                            if (checked)
                                zone.heaterMode = WindowControl.AutoHeater
                        }
                    }
                }
            }
        }
    }
}
