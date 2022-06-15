// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Example.If.AddressBookModule

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("QtIF AddressBook")

    UiAddressBook {
        id: addressBook
    }
    GroupBox {
        anchors.fill: parent
        anchors.margins: 10
        title: "Contacts"

        ColumnLayout {
            anchors.fill: parent
            //! [0]
            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: addressBook.contacts
                clip: true

                delegate: ItemDelegate {
                    width: parent.width
                    height: 100
                    text: model.item.forename + " " + model.item.name
                }
            }
            //! [0]
            Button {
                Layout.fillWidth: true
                text: "New Contact"
                onClicked: addressBook.insertContact(0, AddressBookModule.contact("Foo", "Bar", "12234"))
            }
        }
    }
}
