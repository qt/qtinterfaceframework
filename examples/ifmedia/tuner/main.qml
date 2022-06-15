// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtInterfaceFramework
import QtInterfaceFramework.Media

ApplicationWindow {
    visible: true
    width: 1000
    height: 500
    title: qsTr("Tuner")

    //![1]
    AmFmTuner {
        id: tuner

        band: bandGroup.checkedButton.text === "AM" ? AmFmTuner.AMBand : AmFmTuner.FMBand

        onScanStarted: {
            console.log("A Station SCAN has been started")
        }

        onScanStopped: {
            console.log("A Station SCAN has been stopped")
        }
    }
    //![1]

    SplitView {
        id: splitView1
        anchors.fill: parent

        //![2]
        ColumnLayout {

            RowLayout {
                Label { text: "Station:" }
                Label { text: tuner.station.stationName }
            }

            RowLayout {
                Label { text: "Frequency:" }
                Label { text: tuner.frequency }
            }

            RowLayout {
                Label { text: "Band:" }
                RadioButton { text: "AM"; ButtonGroup.group: bandGroup; checked: tuner.band === AmFmTuner.AMBand }
                RadioButton { text: "FM"; ButtonGroup.group: bandGroup; checked: tuner.band === AmFmTuner.FMBand }
            }

            GroupBox {
                title: "Band settings"
                ColumnLayout {
                    RowLayout {
                        Label { text: "Step Size:" }
                        Label { text: tuner.stepSize }
                    }

                    RowLayout {
                        Label { text: "Minimum Frequency:" }
                        Label { text: tuner.minimumFrequency }
                    }

                    RowLayout {
                        Label { text: "Maximum Frequency::" }
                        Label { text: tuner.maximumFrequency }
                    }
                }
            }

            ButtonGroup {
                id: bandGroup
            }

            RowLayout {
                Button {
                    text: "<-"
                    onClicked: tuner.seekDown()
                }

                Button {
                    text: "<"
                    onClicked: tuner.stepDown()
                }

                Button {
                    text: "Scan"
                    checkable: true
                    onClicked: {
                        if (checked)
                            tuner.startScan();
                        else
                            tuner.stopScan();
                    }
                }

                Button {
                    text: ">"
                    onClicked: tuner.stepUp()
                }

                Button {
                    text: "->"
                    onClicked: tuner.seekUp()
                }
            }

            Item {
                Layout.fillHeight: true
            }

        }
        //![2]

        //![3]
        ListView {
            spacing: 8
            clip: true

            width: 300
            Layout.fillHeight: true

            //![4]
            model: FilterAndBrowseModel {
                serviceObject: tuner.serviceObject
                contentType: "station"
            }
            //![4]

            delegate: Rectangle {
                width: ListView.view.width
                height: column.height
                color: "#efefef"

                //![5]
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        tuner.tune(model.item)
                    }
                }
                //![5]

                Row {
                    anchors.fill: parent
                    Column {
                        id: column
                        width: parent.width * 9 / 10

                        Text { text: "Name: " + model.item.stationName }
                        Text { text: "Type: " + model.item.frequency }
                    }

                    //![6]
                    Button {
                        id: addButton
                        text: "+"
                        width: parent.width / 10
                        height: parent.height

                        onClicked: {
                            presetsModel.insert(0, model.item)
                        }

                        function checkExists() {
                            presetsModel.indexOf(model.item).then(function (index) {
                                addButton.enabled = (index === -1)
                            })
                        }

                        Component.onCompleted: {
                            checkExists()
                        }

                        Connections {
                            target: presetsModel
                            function onCountChanged() {
                                addButton.checkExists()
                            }
                        }
                    }
                    //![6]
                }
            }
        }
        //![3]

        //![7]
        ListView {
            spacing: 8
            clip: true
            Layout.fillWidth: true

            model: FilterAndBrowseModel {
                id: presetsModel
                serviceObject: tuner.serviceObject
                contentType: "presets"
            }

            delegate: Rectangle {
                width: ListView.view.width
                height: column.height
                color: "#efefef"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        tuner.tune(model.item)
                    }
                }

                Row {
                    anchors.fill: parent
                    Column {
                        id: column
                        width: parent.width * 7 / 10

                        Text { text: "Name: " + model.item.stationName }
                        Text { text: "Type: " + model.item.frequency }
                    }

                    Button {
                        text: "\u2227"
                        width: parent.width / 10
                        height: parent.height

                        enabled: index > 0

                        onClicked: {
                            presetsModel.move(index, index - 1)
                        }
                    }

                    Button {
                        text: "\u2228"
                        width: parent.width / 10
                        height: parent.height

                        enabled: index < presetsModel.count -1

                        onClicked: {
                            presetsModel.move(index, index + 1)
                        }
                    }

                    Button {
                        text: "X"
                        width: parent.width / 10
                        height: parent.height

                        onClicked: {
                            presetsModel.remove(index)
                        }
                    }
                }
            }
        }
        //![7]
    }
}
