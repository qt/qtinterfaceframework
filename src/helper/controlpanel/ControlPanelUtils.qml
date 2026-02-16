// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

pragma Singleton
import QtQuick

/*!
    \qmltype ControlPanelUtils
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \ingroup controlpanel-singletons
    \brief A singleton providing helper functions for type conversion
    and default value creation.

    ControlPanelUtils is used internally by the control panel controls
    to parse user input into the correct JavaScript type and to create
    default values when new items are added to lists or models.

    \section1 Functions

    \list
        \li \l parseValue() \unicode{0x2013} converts a string to the
            JavaScript type indicated by a type name (\c "int", \c "real",
            \c "bool", or \c "string").
        \li \l getDefaultValue() \unicode{0x2013} returns a sensible
            default for a given type, including recursive struct
            construction.
    \endlist

    \section1 Usage

    \qml
    var intVal = ControlPanelUtils.parseValue("42", "int")   // 42
    var defVal = ControlPanelUtils.getDefaultValue("bool")   // false
    \endqml
*/

QtObject {

    /*!
        Parses the string \a text into the JavaScript type specified
        by \a type. Returns \c 0 for invalid \c "int" or \c "real"
        input.
    */
    function parseValue(text, type) {
        switch (type) {
            case "int":
                var i = parseInt(text);
                return isNaN(i) ? 0 : i;
            case "real":
                var f = parseFloat(text);
                return isNaN(f) ? 0.0 : f;
            case "bool":
                return text.toLowerCase() === "true" || text === "1";
            case "string":
            default:
                return text;
        }
    }

    /*!
        Returns a sensible default value for the given \a type.
        For \c "struct" types, \a structFields is used to recursively
        build an object. For \c "enum" types, the first entry in
        \a enumModel is returned.
    */
    function getDefaultValue(type, structFields, enumModel) {

        if (structFields === undefined) structFields = null;
        if (enumModel === undefined) enumModel = null;

        switch (type) {
            case "bool":
                return false;
            case "int":
                return 0;
            case "real":
                return 0.0;
            case "enum":
                return enumModel && enumModel.length > 0 ? enumModel[0] : 0;
            case "struct":
                // For struct type
                if (structFields && structFields.length > 0) {
                    var obj = {};
                    for (var i = 0; i < structFields.length; i++) {
                        var field = structFields[i];
                        obj[field.name] = getDefaultValue(field.type, field.structFields, field.enumModel);
                    }
                    return obj;
                }
                return "";
            default:
                return "";
        }
    }
}
