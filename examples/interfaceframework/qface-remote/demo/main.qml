// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Window
//! [0]
import Example.If.RemoteModule

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("QtIF Remote example")

    UiProcessingService {
        id: processingService
    }
//! [0]

    Column {
        anchors.fill: parent
        anchors.margins: 10

        Row {
            //! [1]
            Button {
                text: "Process"

                onClicked: processingService.process(inputField.text).then(
                    function(result) { //success callback
                        resultLabel.text = result
                    },
                    function() { //failure callback
                        resultLabel.text = "failed"
                    } )
            }
            //! [1]
            TextField {
                placeholderText: "text to process"
                id: inputField
            }
        }

        Row {
            Text { text: "Processing result: " }
            Text { id: resultLabel }
        }
        //! [2]
        Row {
            Text { text: "Last message: " }
            Text {
                id: serverMessage
                text: processingService.lastMessage
            }
        }
        //! [2]
    }
}
