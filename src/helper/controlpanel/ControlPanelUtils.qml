// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

pragma Singleton
import QtQuick

/*!
    \qmltype ControlPanelUtils
    \inqmlmodule QtInterfaceFramework.ControlPanelHelper
    \brief Utility functions.

    Provides helper functions for type conversion and default value creation
    used across control panel components.
*/

QtObject {
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
