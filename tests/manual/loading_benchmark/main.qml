// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick
import QtQuick.Window
import Benchmark 1.0

Window {
    width: 500
    height: 500
    color: "blue"
    visible: true

    Loader {
        sourceComponent: Interface0 {
            discoveryMode: "LoadOnlySimulationBackends"
        }
        asynchronous: true
        onLoaded: {
            console.log("loaded")
        }
    }

    Component.onCompleted:  {
        console.log("completed")
    }
}
