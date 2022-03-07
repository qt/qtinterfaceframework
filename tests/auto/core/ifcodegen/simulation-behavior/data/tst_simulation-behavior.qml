// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtTest
import QtQml
import Simu

TestCase {
    SomeInterface {
        id: someInterface
        onIsInitializedChanged: (val) => {
            verify(val);
            addOne(41).then(function(result) {
                compare(result, 42);
            }, function() {
                verify(false);
            });
        }
    }

    SignalSpy {
        id: isInitializedChangedSpy
        target: someInterface
        signalName: "isInitializedChanged"
    }

    // Calling a function directly from the
    // onIsInitializedChanged handler should
    // be possible and not prevented by the
    // recursion guard in the simulation engine.
    function test_immediateCall() {
        isInitializedChangedSpy.wait();
        wait(200);
    }

    // The property value is hardcoded in the
    // simulation. The setter causes a recursion
    // which should be detected by the recursion guard
    // The onTestProeprtyChanged handler is setting it
    // again, causing the same behavior to be executed
    // and should cause another (wanted) recursion
    // until the property is settled as it's already correct.
    Connections {
        id: hpConnections
        target: someInterface
        enabled: false
        property bool changeSignalCalled: false
        function onHardcodedPropertyChanged() {
            changeSignalCalled = true;
            console.log("setHardcodedProperty 10")
            someInterface.hardcodedProperty = 10;
        }
    }

    function test_allowCallsInOnChangedHandler() {
        hpConnections.enabled = true
        console.log("init hardcodedProperty 10")
        someInterface.hardcodedProperty = 10;
        // This is the value hardcoded in the backend
        // If the recursion guard doesn't work correctly,
        // the second call doesn't hit the simulation and
        // would cause the backend to change the value to 10.
        compare(someInterface.hardcodedProperty, 1);
        compare(hpConnections.changeSignalCalled, true);
    }

    // The simulation updates the same property multiple times
    // all those calls would mean a recursion and need to be prevented
    function test_preventRecursionOnMultipleSimulationCalls() {
        console.log("init otherHardcodedProperty 10")
        someInterface.otherHardcodedProperty = 10;
        compare(someInterface.otherHardcodedProperty, 2);
    }
}
