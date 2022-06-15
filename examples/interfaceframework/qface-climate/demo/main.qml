// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Window
//! [0]
import Example.If.ClimateModule

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("QtIF Climate")

    UiClimateControl {
        id: climateCtrl
    }

    Column {
        anchors.fill: parent
        anchors.margins: 5

        Text {
            text: "Air Conditioning: " + climateCtrl.airConditioning
        }
//! [0]
        Text {
            text: "Heater: " + climateCtrl.heater
        }
        Text {
            text: "Fan Speed Level: " + climateCtrl.fanSpeedLevel
        }
        Text {
            text: "Steering Wheel Heater: " + climateCtrl.steeringWheelHeater
        }
        Text {
            text: "Target Temperature: " + climateCtrl.targetTemperature
        }
        Text {
            text: "Seat Cooler: " + climateCtrl.seatCooler
        }
        Text {
            text: "Seat Heater: " + climateCtrl.seatHeater
        }
        Text {
            text: "Outside Temperature: " + climateCtrl.outsideTemperature
        }
        Text {
            text: "Zone Synchronization: " + climateCtrl.zoneSynchronization
        }
        Text {
            text: "Defrost: " + climateCtrl.defrost
        }
        Text {
            property var vals: ["Off", "On", "Auto"]
            text: "Recirculation Mode: " + vals[climateCtrl.recirculationMode]
        }
        Text {
            text: "Recirculation: " + climateCtrl.recirculation
        }
        Text {
            text: "Recirculation Sensitivity Level: " + climateCtrl.recirculationSensitivityLevel
        }
        Text {
            property var vals: ["Off", "On", "Auto"]
            text: "Climate Mode: " + vals[climateCtrl.climateMode]
        }
        Text {
            text: "Automatic Climate Fan Intensity Level: " + climateCtrl.automaticClimateFanIntensityLevel
        }
    }
}
