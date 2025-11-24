// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick

/*!
    \qmltype CustomControlPanelComponent
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper

    \brief Base class for custom property components.

    All custom property UI components must inherit from this base class.
    It provides a standard interface that the Control Panel uses to communicate
    with custom components.
*/
Item {
    id: root

    property var backendObject: null
    property var backendProperty: null
    property string propertyName: ""
    property string setterFunction: ""
    property bool isZoned: false
    property string currentZone: ""

    function setValue(value) {
        if (!backendObject || !setterFunction) {
            console.warn("CustomControlPanelComponent: Cannot set value - backend not initialized")
            return
        }

        var setter = backendObject[setterFunction]
        if (typeof setter !== "function") {
            console.error("CustomControlPanelComponent: Setter function '" + setterFunction + "' not found")
            return
        }

        if (isZoned && currentZone !== "") {
            setter(value, currentZone)
        } else {
            setter(value)
        }
    }
}
