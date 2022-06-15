// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

//! [0]
import QtQuick
import Example.If.AddressBookModule.simulation

Item {
    AddressBookBackend {
        id: backend
        property var settings : IfSimulator.findData(IfSimulator.simulationData, "AddressBook")

        function initialize() {
            print("AddressBookSimulation INITIALIZE")
            IfSimulator.initializeDefault(settings, backend)
            Base.initialize()
        }

        function insertContact(reply, index, contact) {
            print("BACKEND SIMULATION INSERT CONTACT")
            contacts.insert(index, contact);
            reply.setSuccess(true);
        }

        Component.onCompleted: {
            console.log("BACKEND SIMULATION CREATED")
        }
    }
}
//! [0]
