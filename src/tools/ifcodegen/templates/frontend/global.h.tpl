{#
#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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
{% set exportsymbol = '{0}'.format(module|upper|replace('.', '_')) %}
{% set oncedefine = '{0}GLOBAL_H_'.format(exportsymbol) %}
{% include 'common/generated_comment.cpp.tpl' %}

#ifndef {{oncedefine}}
#define {{oncedefine}}

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_{{exportsymbol|strip_QT}}_LIB)
#    define Q_{{exportsymbol}}_EXPORT Q_DECL_EXPORT
#  else
#    define Q_{{exportsymbol}}_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_{{exportsymbol}}_EXPORT
#endif

QT_END_NAMESPACE

#endif // {{oncedefine}}
