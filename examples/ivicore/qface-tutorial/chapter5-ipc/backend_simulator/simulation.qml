/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

import QtQuick 2.10
import Example.IVI.InstrumentCluster.simulation 1.0

QtObject {
    property var settings : IviSimulator.findData(IviSimulator.simulationData, "InstrumentCluster")
    property bool defaultInitialized: false
    property LoggingCategory qLcInstrumentCluster: LoggingCategory {
        name: "example.ivi.instrumentcluster.simulation.instrumentclusterbackend"
    }
    property var backend : InstrumentClusterBackend {

        function initialize() {
            console.log(qLcInstrumentCluster, "INITIALIZE")
            if (!defaultInitialized) {
                IviSimulator.initializeDefault(settings, backend)
                defaultInitialized = true
            }
            Base.initialize()
        }

        property int gearSpeed: 260 / 6
        property int currentGear: speed / gearSpeed
        rpm : currentGear >= 1 ? 3000 + (speed % gearSpeed) / gearSpeed * 2000
                               : (speed % gearSpeed) / gearSpeed * 5000

        property var animation: SequentialAnimation {
            loops: Animation.Infinite
            running: true

            ParallelAnimation {
                SequentialAnimation {

                    NumberAnimation {
                        target: backend
                        property: "speed"
                        from: 0
                        to: 80
                        duration: 4000
                    }

                    NumberAnimation {
                        target: backend
                        property: "speed"
                        to: 50
                        duration: 2000
                    }

                    NumberAnimation {
                        target: backend
                        property: "speed"
                        to: 200
                        duration: 7000
                    }

                    ScriptAction {
                        script: {
                            backend.currentWarning = InstrumentCluster.warning("red","LOW FUEL", "images/fuelsymbol_orange.png")
                        }
                    }

                    NumberAnimation {
                        target: backend
                        property: "speed"
                        to: 0
                        duration: 5000
                    }

                    ScriptAction {
                        script: {
                            backend.currentWarning = InstrumentCluster.warning()
                        }
                    }
                }

                NumberAnimation {
                    target: backend
                    property: "fuel"
                    from: 1
                    to: 0
                }
            }

            NumberAnimation {
                target: backend
                property: "fuel"
                from: 0
                to: 1
                duration: 4000
            }
        }
    }
}

