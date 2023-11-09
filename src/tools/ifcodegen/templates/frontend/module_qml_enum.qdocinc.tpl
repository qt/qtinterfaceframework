{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# SPDX-License-Identifier: LicenseRef-Qt-Commercial
#}

{% for enum in module.enums %}
//! [{{enum}}]
{%   for member in enum.members %}
{%     with doc = member.comment|parse_doc %}
\value {{member.name}}
{%       if doc.description %}
       {{doc.description|join(' ')| wordwrap(width=100, wrapstring='\n       ')}}
{%       endif %}
{%     endwith %}
{%   endfor %}
//! [{{enum}}]
{% endfor %}
