// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import Example.If.InstrumentClusterModule.simulation

QtObject {
    property var settings : IfSimulator.findData(IfSimulator.simulationData, "InstrumentCluster")
    property bool defaultInitialized: false
    property LoggingCategory qLcInstrumentCluster: LoggingCategory {
        name: "example.if.instrumentclustermodule.simulation.instrumentclusterbackend"
    }
    property var backend : InstrumentClusterBackend {

        function initialize() {
            console.log(qLcInstrumentCluster, "INITIALIZE")
            if (!defaultInitialized) {
                IfSimulator.initializeDefault(settings, backend)
                defaultInitialized = true
            }
            Base.initialize()
        }

        property int gearSpeed: 260 / 6
        property int currentGear: speed / gearSpeed
        rpm: currentGear >= 1 ? 3000 + (speed % gearSpeed) / gearSpeed * 2000
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
                            backend.currentWarning = InstrumentClusterModule.warning("red","LOW FUEL", "images/fuelsymbol_orange.png")
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
                            backend.currentWarning = InstrumentClusterModule.warning()
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

