{#
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% import 'common/qtif_macros.j2' as qtif %}
{% include "common/generated_comment.qml.tpl" %}
{% set module_qml_name = (module|qml_type).split('.')[-1]|upperfirst %}
{% set default_category_name = module_qml_name %}
{% if module.tags.designer and module.tags.designer.categoryName %}
{%   set default_category_name = module.tags.designer.categoryName %}
{% endif %}

MetaInfo {
{% for interface in module.interfaces %}
{%   set category_name = default_category_name %}
{%   if interface.tags.designer and interface.tags.designer.categoryName %}
{%     set category_name = interface.tags.designer.categoryName %}
{%   endif %}
{%   set name = interface|qml_type %}
{%   if interface.tags.designer and interface.tags.designer.name %}
{%     set name = interface.tags.designer.name %}
{%   endif %}

   Type {
        name: "{{module|qml_type}}.{{interface|qml_type}}"
{%   if interface.tags.designer and interface.tags.designer.typeIcon %}
        icon: "{{interface.tags.designer.typeIcon}}"
{%   endif %}

        Hints {
            visibleInNavigator: true
            canBeDroppedInNavigator: true
            canBeDroppedInFormEditor: false
        }

        ItemLibraryEntry {
            name: "{{name}}"
            category: "{{category_name}}"
{%   if interface.tags.designer and interface.tags.designer.libraryIcon %}
             libraryIcon: "{{interface.tags.designer.libraryIcon}}"
{%   endif %}
            version: "{{module.majorVersion}}.{{module.minorVersion}}"
            requiredImport: "{{module|qml_type}}"
        }
    }

{% endfor %}
}
