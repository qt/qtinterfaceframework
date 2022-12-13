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
    width: 800
    height: 600
    title: qsTr("Media Player")

    MediaPlayer {
        id: mediaPlayer
    }

    SplitView {
        id: splitView1
        anchors.fill: parent

        ColumnLayout {

            RowLayout {
                Label { text: "Artist:" }
                Label { text: mediaPlayer.currentTrack ? mediaPlayer.currentTrack.artist : "" }
            }

            RowLayout {
                Label { text: "Album:" }
                Label { text: mediaPlayer.currentTrack ? mediaPlayer.currentTrack.album : "" }
            }

            RowLayout {
                Label { text: "Title:" }
                Label { text: mediaPlayer.currentTrack ? mediaPlayer.currentTrack.title : "" }
            }

            RowLayout {
                Button {
                    text: "<-"
                    onClicked: mediaPlayer.previous()
                }

                Button {
                    text: "stop"
                    onClicked: mediaPlayer.stop()
                }

                Button {
                    text: ">"
                    onClicked: mediaPlayer.play()
                }

                Button {
                    text: "->"
                    onClicked: mediaPlayer.next()
                }
            }

            ListView {
                spacing: 8
                clip: true
                Layout.fillHeight: true
                Layout.fillWidth: true

                model: mediaPlayer.playQueue

                delegate: Rectangle {
                    width: ListView.view.width
                    height: column.height
                    color: mediaPlayer.playQueue.currentIndex == index ? "lightblue" : "#efefef"

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            mediaPlayer.playQueue.currentIndex = index
                            mediaPlayer.play()
                        }
                    }

                    Row {
                        anchors.fill: parent
                        Column {
                            id: column
                            width: parent.width * 7 / 10

                            Text { text: "Index: " + index }
                            Text { text: "Name: " + model.name }
                            Text { text: "Type: " + model.item.type }
                        }

                        Button {
                            text: "\u2227"
                            width: parent.width / 10
                            height: parent.height

                            enabled: index > 0

                            onClicked: {
                                mediaPlayer.playQueue.move(index, index - 1)
                            }
                        }

                        Button {
                            text: "\u2228"
                            width: parent.width / 10
                            height: parent.height

                            enabled: index < mediaPlayer.playQueue.count -1

                            onClicked: {
                                mediaPlayer.playQueue.move(index, index + 1)
                            }
                        }

                        Button {
                            text: "X"
                            width: parent.width / 10
                            height: parent.height

                            onClicked: {
                                mediaPlayer.playQueue.remove(index)
                            }
                        }
                    }

                }
            }
        }

        ColumnLayout {

            RowLayout {
                Label { text: "filter:" }
                TextField { id: filterEdit }
                Button { text: "Refresh"; onClicked: searchModel.reload(); }
            }

            ListView {
                spacing: 8
                clip: true
                Layout.fillHeight: true
                Layout.minimumWidth: 300

                header: Rectangle {
                    width: ListView.view.width
                    height: searchModel.canGoBack ? 50 : 0
                    visible: height > 0
                    color: "#efefef"

                    Text {
                        anchors.centerIn: parent
                        text: "back"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            searchModel.goBack()
                        }
                    }
                }

                model: FilterAndBrowseModel {
                    id: searchModel
                    serviceObject: mediaPlayer.serviceObject
                    contentType: "artist"
                    loadingType: FilterAndBrowseModel.FetchMore
                    query: filterEdit.text
                    onQueryChanged: filterEdit.text = query
                }

                delegate: Rectangle {
                    width: ListView.view.width
                    height: column.height
                    color: "#efefef"

                    Column {
                        id: column
                        width: parent.width

                        Text { text: "Name: " + model.name }
                        Text { text: "Type: " + model.item.type }
                        Text { text: "CanForward: " + model.canGoForward }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (item.type === "audiotrack")
                                mediaPlayer.playQueue.insert(0, item)
                            else
                                searchModel.goForward(index, FilterAndBrowseModel.InModelNavigation)
                        }
                    }
                }
            }
        }

        ColumnLayout {
            ListView {
                id: browseView
                spacing: 8
                clip: true
                Layout.fillHeight: true
                Layout.minimumWidth: 300

                header: Rectangle {
                    width: ListView.view.width
                    height: browseView.model !== discoveryModel ? 50 : 0
                    visible: height > 0
                    color: "#efefef"

                    Text {
                        anchors.centerIn: parent
                        text: "back"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (filterModel.canGoBack)
                                filterModel.goBack()
                            else
                                browseView.model = discoveryModel
                        }
                    }
                }

                model: MediaDeviceDiscoveryModel {
                    id: discoveryModel

                    onDeviceRemoved: {
                        if (device === filterModel.serviceObject)
                            browseView.model = discoveryModel
                    }
                }

                FilterAndBrowseModel {
                    id: filterModel
                    contentType: "file"
                }

                delegate: Rectangle {
                    width: ListView.view.width
                    height: column.height
                    color: "#efefef"

                    Column {
                        id: column
                        width: parent.width

                        Text { text: "Name: " + model.name }
                        Text { text: "Type: " + model.type }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (browseView.model === discoveryModel) {
                                filterModel.serviceObject = model.serviceObject
                                browseView.model = filterModel
                                return;
                            }

                            filterModel.goForward(index, FilterAndBrowseModel.InModelNavigation)
                        }
                    }
                }
            }
        }
    }

    InterfaceFrameworkConfiguration {
        name: "qtifmedia"
        discoveryMode: AbstractFeature.LoadOnlySimulationBackends

// Enable this to pass settings to the simulation backend
//        serviceSettings: {
//            "useTemporaryDatabase": true
//        }
    }
}
