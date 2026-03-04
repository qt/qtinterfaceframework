// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

import QtQuick

/*!
    \qmltype CustomControlPanelComponent
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-components
    \brief Base type for user-defined property editors in the control panel.

    \techpreview

    All custom property UI components must inherit from this base type.
    It provides the standard interface that the simulation control panel
    uses to supply backend context and zone information. Subclasses can
    call the \l setValue() function to write back to the backend.

    The properties \l backendObject, \l backendProperty, \l propertyName,
    \l setterFunction, \l isZoned, and \l currentZone are populated
    automatically by the control panel when the component is loaded.

    \note This type is only needed when the built-in controls are not
    sufficient and you want to provide a custom property editor.

    \section1 Usage

    \qml
    import QtInterfaceFramework.ControlPanelHelper

    CustomControlPanelComponent {
        id: myEditor
        // backendObject, propertyName, setterFunction, etc.
        // are set automatically by the control panel.

        Slider {
            value: myEditor.backendProperty
            onMoved: myEditor.setValue(value)
        }
    }
    \endqml
*/
Item {
    id: root

    /*!
        This property holds a reference to the backend object that
        owns the property being edited.
    */
    property var backendObject: null

    /*!
        This property holds the current value of the property being edited.
        Bind the editor's input control to this to reflect the backend state.
    */
    property var backendProperty: null

    /*!
        This property holds the name of the property being edited.
    */
    property string propertyName: ""

    /*!
        This property holds the name of the setter function on the
        backend object used by \l setValue() to write values back.
    */
    property string setterFunction: ""

    /*!
        This property holds whether the property belongs to a zoned
        interface. When \c true, \l setValue() passes \l currentZone
        as an additional argument.
    */
    property bool isZoned: false

    /*!
        This property holds the currently selected zone identifier.
    */
    property string currentZone: ""

    /*!
        Writes \a value back to the backend by calling the function
        named in \l setterFunction. When \l isZoned is \c true, the
        \l currentZone is passed as a second argument.
    */
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
