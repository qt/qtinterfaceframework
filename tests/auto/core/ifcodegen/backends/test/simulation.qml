// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick
import org.example.echomodule.simulation
import "qrc:///simulation"
import cmdlistener

QtObject {
    property var echo : EchoSimulation {
        id: echo

        property var timerComp: Component {
            Timer {
                running: true
                repeat: false
            }
        }
        function timer(reply, interval) {
            var timer = timerComp.createObject(echo, {
                                                   interval: interval,
                                               })
            timer.triggered.connect(() => { reply.setSuccess(true); })
            return reply;
        }

        function echo(reply, msg) {
            reply.setSuccess(msg);
        }

        function id(reply) {
            reply.setSuccess("id123");
        }

        function getCombo(reply) {
            reply.setSuccess(Echomodule.combo(Echomodule.contact("Antti", 34, true, undefined), Echomodule.Friday));
        }

        function voidSlot(reply) {
            reply.setSuccess(true);
        }

        function voidSlot2(reply, param) {
            reply.setSuccess(true);
        }

        function flagMethod(reply, direction) {
            reply.setSuccess(direction);
        }

        function enumMethod(reply, testEnum) {
            reply.setSuccess(testEnum);
        }
    }
    property var echozoned : EchoZonedSimulation {
        id: echozoned

        property var timerComp: Component {
            Timer {
                running: true
                repeat: false
            }
        }
        function timer(reply, interval, zone) {
            var timer = timerComp.createObject(echo, {
                                                   interval: interval,
                                               })
            timer.triggered.connect(() => { reply.setSuccess(zone); })
            return reply;
        }

        function echo(reply, msg, zone) {
            reply.setSuccess(msg);
        }

        function id(reply, zone) {
            reply.setSuccess("id123");
        }

        function varMethod(reply, zone) {
            reply.setSuccess("FOOO");
        }

        function getCombo(reply, zone) {
            reply.setSuccess(Echomodule.combo(Echomodule.contact("Antti", 34, true, undefined), Echomodule.Friday));
        }

        function flagMethod(reply, direction, zone) {
            reply.setSuccess(direction);
        }

        function enumMethod(reply, testEnum, zone) {
            reply.setSuccess(testEnum);
        }
    }

    property var cmdListener: CmdListener {
        onNewCmd: (cmd) => {
            //console.log("NEW CMD", cmd);
            handleCmd(cmd)
        }
    }

    function handleCmd(cmd) {
        if (cmd === "testInit") {
            echo.lastMessage = "this is the last message";
            echo.intValue = 789;
            echo.floatValue1 = 3.14;
            echo.floatValue2 = 2.71;
            echo.stringValue = "test string";
            echo.comboList = [ Echomodule.combo(Echomodule.contact("Mr A.", 20, false, "foo"), Echomodule.Monday),
                               Echomodule.combo(Echomodule.contact("Mr B.", 40, true, "bar"), Echomodule.Wednesday)]
            echo.contact = Echomodule.contact("Nemo", 47, true, 1)
            echo.weekDay = Echomodule.Wednesday;
            echo.testEnum = Echomodule.SecondEnumValue;
        } else if (cmd === "testZonedInit") {
            echozoned.intValue = 789;
            echozoned.varValue = 789;
            echozoned.stringValue = "test string";

            echozoned.zoneAt("FrontLeft").airflowDirection = Echomodule.Windshield;
            echozoned.zoneAt("FrontLeft").comboList = [ Echomodule.combo(Echomodule.contact("Mr A.", 20, false, "foo"), Echomodule.Monday),
                                                        Echomodule.combo(Echomodule.contact("Mr B.", 40, true, "bar"), Echomodule.Wednesday)]
            echozoned.zoneAt("FrontLeft").contact = Echomodule.contact("Nemo", 47, true, 1)
            echozoned.zoneAt("FrontLeft").UPPERCASEPROPERTY = 3.14;
            echozoned.zoneAt("FrontLeft").testEnum = Echomodule.SecondEnumValue;
        } else if (cmd === "changeProperties") {
            echo.intValue = 12345;
            echo.floatValue1 = 1234.5678;
            echo.floatValue2 = 234.678;
            echo.stringValue = "hello test";
            echo.comboList = [ Echomodule.combo(Echomodule.contact("Mr A.", 20, false, "foo"), Echomodule.Monday),
                            Echomodule.combo(Echomodule.contact("Mr B.", 40, true, "bar"), Echomodule.Wednesday)]
            echo.contact = Echomodule.contact("Nemo", 47, true, 1)
            echo.weekDay = Echomodule.Friday;
            echo.testEnum = Echomodule.SecondEnumValue;
        } else if (cmd === "changeZoneProperties") {
            echozoned.zoneAt("FrontLeft").stringValue = "hello test";

            echozoned.zoneAt("FrontLeft").airflowDirection = Echomodule.Windshield;
            echozoned.zoneAt("FrontLeft").comboList = [ Echomodule.combo(Echomodule.contact("Mr A.", 20, false, "foo"), Echomodule.Thursday),
                                                        Echomodule.combo(Echomodule.contact("Mr B.", 40, true, "bar"), Echomodule.Tuesday)]
            echozoned.zoneAt("FrontLeft").contact = Echomodule.contact("Nemo", 47, true, 1)
            echozoned.zoneAt("FrontLeft").testEnum = Echomodule.SecondEnumValue;
            echozoned.UPPERCASEPROPERTY = 1234.5678;
            echozoned.intValue = 12345;
        } else if (cmd === "emitSignals") {
            echozoned.somethingHappened("FrontLeft");
            echozoned.foobar("foo and bar", "FrontLeft");
            echozoned.anotherChanged(Echomodule.anotherStruct(7), "");
            echo.somethingHappened();
            echo.foobar("foo and bar");
            echo.anotherChanged(Echomodule.anotherStruct(7));
        } else if (cmd === "insert") {
            echo.contactList.insert(0, Echomodule.contact("Mr A.", 20, false, "foo"))
        } else if (cmd === "update") {
            echo.contactList.update(0, Echomodule.contact("Mr B.", 30, true, undefined))
        } else if (cmd === "remove") {
            echo.contactList.remove(0)
        }
    }
}
