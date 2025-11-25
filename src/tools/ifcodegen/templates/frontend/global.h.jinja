{#
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
#}
{% set exportsymbol = '{0}'.format(module|upper|replace('.', '_')) %}
{% set oncedefine = '{0}GLOBAL_H_'.format(exportsymbol) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QtCore/qglobal.h>

{{ module|begin_namespace }}

#ifndef QT_STATIC
#  if defined(QT_BUILD_{{exportsymbol|strip_QT}}_LIB)
#    define Q_{{exportsymbol}}_EXPORT Q_DECL_EXPORT
#  else
#    define Q_{{exportsymbol}}_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_{{exportsymbol}}_EXPORT
#endif

{{ module|end_namespace }}

#endif // {{oncedefine}}
