/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: BSD-3-Clause
**
****************************************************************************/

import QtQuick 2.7
import QtQuick.Window 2.2
//! [0]
import IviClimate 1.0

Window {

    visible: true
    width: 640
    height: 480
    title: qsTr("QtIVI Climate")

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
