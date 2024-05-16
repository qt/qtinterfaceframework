// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick
import QtInterfaceFramework

QtObject {
    id: root
    signal loadingSucceeded();

    function findAndLoad(interfaceName : string, async : bool) {
        var handles = ServiceManager.findServiceHandleByInterface(interfaceName)
        console.assert(handles.length === 1)
        console.assert(handles[0].valid)
        console.assert(!handles[0].loaded)

        ServiceManager.loadServiceObject(handles[0], async);
    }

    property Connections con: Connections {
        target: ServiceManager
        function onServiceObjectLoaded(serviceHandle) {
            console.assert(serviceHandle.valid)
            console.assert(serviceHandle.loaded)
            console.assert(serviceHandle.serviceObject !== null)
            root.loadingSucceeded();
        }
    }
}
