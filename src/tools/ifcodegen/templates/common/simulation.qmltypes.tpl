{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2019 Luxoft Sweden AB
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:COMM$
##
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## $QT_END_LICENSE$
##
##
##
##
##
##
##
##
#############################################################################
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% include "common/generated_comment.qml.tpl" %}
{% set module_qml_name = (module|qml_type).split('.')[-1]|upperfirst %}
import QtQuick.tooling 1.2

// This file describes the plugin-supplied types contained in the library.
// It is used for QML tooling purposes only.
//
// This file was auto-generated with the QtInterfaceFramework ifcodegen

Module {
    dependencies: ["QtInterfaceFramework 1.0"]
    Component {
        name: "IfSimulator"
        prototype: "QIfSimulationGlobalObject"
        exports: ["{{module|qml_type}}.simulation/IfSimulator {{module.majorVersion}}.{{module.minorVersion}}"]
        isCreatable: false
        isSingleton: true

        Property { name: "simulationData"; type: "QVariantMap" }
        Method {
            name: "findData"
            type: "QVariantMap"
            Parameter { name: "data"; type: "QVariantMap" }
            Parameter { name: "interface"; type: "string" }
        }
        Method {
            name: "initializeDefault"
            Parameter { name: "data"; type: "QVariantMap" }
            Parameter { name: "object"; type: "QObject"; isPointer: true }
        }
        Method {
            name: "defaultValue"
            type: "QVariant"
            Parameter { name: "data"; type: "QVariantMap" }
            Parameter { name: "zone"; type: "string" }
        }
        Method {
            name: "constraint"
            type: "string"
            Parameter { name: "data"; type: "QVariantMap" }
            Parameter { name: "value"; type: "QVariant" }
        }
        Method {
            name: "checkSettings"
            type: "bool"
            Parameter { name: "data"; type: "QVariantMap" }
            Parameter { name: "value"; type: "QVariant" }
            Parameter { name: "zone"; type: "string" }
        }
        Method {
            name: "parseDomainValue"
            type: "QVariant"
            Parameter { name: "data"; type: "QVariantMap" }
            Parameter { name: "domain"; type: "string" }
            Parameter { name: "zone"; type: "string" }
        }
    }

{% for interface in module.interfaces %}
{%   for property in interface.properties %}
{%     if property.type.is_model %}
    Component {
        name: "{{property|upperfirst}}ModelBackend"
        prototype: "{{property|upperfirst}}ModelBackendInterface"
        exports: ["{{module|qml_type}}.simulation/{{property|upperfirst}}ModelBackend {{module.majorVersion}}.{{module.minorVersion}}"]

        Property { name: "Base"; type: "{{property|upperfirst}}ModelBackend"; isPointer: true }
        Property { name: "count"; type: "int" }

        Method { name: "initialize" }
        Method {
            name: "registerInstance"
            Parameter { name: "identifier"; type: "QUuid" }
        }
        Method {
            name: "unregisterInstance"
            Parameter { name: "identifier"; type: "QUuid" }
        }
        Method {
            name: "fetchData"
            Parameter { name: "identifier"; type: "QUuid" }
            Parameter { name: "start"; type: "int" }
            Parameter { name: "count"; type: "int" }
        }
        Method {
            name: "insert"
            Parameter { name: "index"; type: "int" }
            Parameter { name: "item"; type: "{{property.type.nested}}" }
        }
        Method {
            name: "remove"
            Parameter { name: "index"; type: "int" }
        }
        Method {
            name: "move"
            Parameter { name: "currentIndex"; type: "int" }
            Parameter { name: "newIndex"; type: "int" }
        }
        Method { name: "reset" }
        Method {
            name: "update"
            Parameter { name: "index"; type: "int" }
            Parameter { name: "item"; type: "{{property.type.nested}}" }
        }
        Method {
            name: "at"
            type: "{{property.type.nested}}"
            Parameter { name: "index"; type: "int" }
        }
    }

{%     endif %}
{%   endfor %}
{%   set class = interface %}
{%   set interface_zoned = interface.tags.config and interface.tags.config.zoned %}
{%   if interface_zoned %}
    Component {
        name: "{{class}}Zone"
        prototype: "{{class}}Zone"
        isCreatable: false
        exports: ["{{module|qml_type}}.simulation/{{class}}Zone {{module.majorVersion}}.{{module.minorVersion}}"]
        exportMetaObjectRevisions: [0]
        Property { name: "zones"; type: "QQmlPropertyMap"; isPointer: true }
{%     for property in interface.properties %}
        Property { name: "{{property}}" {% if property.type.is_model %}; type: "{{property|upperfirst}}ModelBackend"; isPointer: true {% else %} ; type: "{{property|qml_info_type}}" {% endif %} }
{%     endfor %}
{%     for property in interface.properties %}
        Signal {
            name: "{{property}}Changed"
            Parameter { name: "{{property}}" {% if property.type.is_model %}; type: "{{property|upperfirst}}ModelBackend"; isPointer: true {% else %} ; type: "{{property|qml_info_type}}" {% endif %} }
        }
{%     endfor %}
{%     for property in interface.properties %}
        Method {
            name: "{{property|setter_name}}"
            Parameter { name: "{{property}}" {% if property.type.is_model %}; type: "{{property|upperfirst}}ModelBackend"; isPointer: true {% else %} ; type: "{{property|qml_info_type}}" {% endif %} }
        }
{%     endfor %}
    }
{%   endif %}
    Component {
        name: "{{class}}Backend"
        prototype: "{{class}}BackendInterface"
        exports: ["{{module|qml_type}}.simulation/{{class}}Backend {{module.majorVersion}}.{{module.minorVersion}}"]
        exportMetaObjectRevisions: [0]
        Property { name: "Base"; type: "{{class}}Backend"; isPointer: true }
{%   for property in interface.properties %}
        Property { name: "{{property}}" {% if property.type.is_model %}; type: "{{property|upperfirst}}ModelBackend"; isPointer: true {% else %} ; type: "{{property|qml_info_type}}" {% endif %} }
{%   endfor %}
{%   for property in interface.properties %}
        Signal {
            name: "{{property}}Changed"
            Parameter { name: "{{property}}"; type: "{{property|qml_info_type}}" }
        }
{%   endfor %}
{%   for signal in interface.signals %}
        Signal {
            name: "{{signal}}"
{%     for parameter in signal.parameters %}
            Parameter { name: "{{parameter}}"; type: "{{parameter|qml_info_type}}" }
{%     endfor %}
        }
{%   endfor %}
        Method { name: "initialize" }
{%   for operation in interface.operations %}
        Method {
            name: "{{operation}}"
{# TODO: Once QtCreator can also provide the completion for return types we need to reevaluate this #}
            type: "QIfPendingReply<{{operation|return_type}}>"
{%     for parameter in operation.parameters %}
            Parameter { name: "{{parameter}}"; type: "{{parameter|qml_info_type}}" }
{%     endfor %}
        }
{%   endfor %}
{%   for property in interface.properties %}
        Method {
            name: "{{property|setter_name}}"
            Parameter { name: "{{property}}"; type: "{{property|qml_info_type}}" }
        }
{%   endfor %}
    }
{% endfor %}

    Component {
        name: "{{module.module_name|upperfirst}}"
        prototype: "QObject"
        exports: ["{{module|qml_type}}.simulation/{{module_qml_name}} {{module.majorVersion}}.{{module.minorVersion}}"]
        isCreatable: false
        isSingleton: true
        exportMetaObjectRevisions: [0]
{% for enum in module.enums %}
        Enum {
            name: "{{enum|flag_type}}"
            values: {
{%   for member in enum.members %}
                "{{member.name}}": {{member.value}}{% if not loop.last %},{% endif %}

{%   endfor %}
            }
        }
{% endfor %}
{% for struct in module.structs %}
        Method {
            name: "{{struct|lowerfirst}}"
            type: "{{struct}}"
{%   for fields in struct.fields %}
            Parameter { name: "{{fields}}"; type: "{{fields|qml_info_type}}" }
{%   endfor %}
        }
{% endfor %}
{% for struct in module.structs %}
        Method {
            name: "{{struct|lowerfirst}}"
            type: "{{struct}}"
        }
{% endfor %}
    }
}
