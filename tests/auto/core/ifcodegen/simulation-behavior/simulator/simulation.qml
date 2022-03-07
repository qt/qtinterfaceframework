// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick
import Simu.simulation

QtObject {
    property bool defaultInitialized: false
    property var backend: SomeInterfaceBackend {
        function initialize() {
            if (!defaultInitialized) {
                IfSimulator.initializeDefault(IfSimulator.findData(IfSimulator.simulationData, "SomeInterface"),
                                              backend);
                defaultInitialized = true;
            }
            Base.initialize();
        }

        function addOne(reply, value) {
            reply.setSuccess(++value);
        }

        function setHardcodedProperty(value) {
            console.log("simu setHardcodedProperty(" + value + ") called")
            console.log("simu hardcoding value to 1")
            Base.setHardcodedProperty(1)
        }

        function setOtherHardcodedProperty(value) {
            console.log("simu setOtherHardcodedProperty(" + value + ") called")
            console.log("simu hardcoding value to 1")
            Base.setOtherHardcodedProperty(1)
            console.log("update other unrelated property")
            simpleProperty = 5;
            console.log("simu hardcoding value to 2")
            Base.setOtherHardcodedProperty(2)
        }
    }
}
