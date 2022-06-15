// Copyright (C) 2021 The Qt Company Ltd.
// Copyright (C) 2019 Luxoft Sweden AB
// Copyright (C) 2018 Pelagicore AG
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick
import QtInterfaceFramework
import testfeature

Item {

    TestFeature {
        objectName: "default"
    }

    TestFeature {
        id: noAutoItem
        objectName: "autoDiscoveryDisabled"
        discoveryMode: AbstractFeature.NoAutoDiscovery
    }

    function discover() {
        // We have to use AbstractFeature here instead of TestFeature because of
        // QTBUG-83703
        noAutoItem.discoveryMode = AbstractFeature.LoadOnlyProductionBackends
        noAutoItem.startAutoDiscovery()
    }
}
