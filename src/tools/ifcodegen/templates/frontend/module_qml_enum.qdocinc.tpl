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
