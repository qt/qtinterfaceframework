#!/usr/bin/env python3
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB)
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import inspect
import click

from jinja2 import TemplateAssertionError

from . import builtin_config

currentSrcFile = ""


def jinjaTrace():
    """
    Collects all jinja template files and the line numbers from the current calltrace
    """
    frame = inspect.currentframe()
    infos = []
    while frame:
        template = frame.f_globals.get('__jinja_template__')
        if template is not None:
            infos.append((inspect.getsourcefile(frame),
                          template.get_corresponding_lineno(frame.f_lineno)))
        frame = frame.f_back
    return infos


def jinja_error(msg):
    """
    Throws an error for the current jinja template and the templates this is included from
    This can be used inside a filter to indicate problems with the passed arguments or direclty
    inside an template
    """
    infos = jinjaTrace()
    if len(infos):
        message = msg
        if len(infos) > 1:
            for info in infos[1:]:
                message = message + "\n{0}:{1}: instantiated from here".format(info[0], info[1])
        message = message + "\n{0}: instantiated from here".format(currentSrcFile)
        raise TemplateAssertionError(message, infos[0][1], "", infos[0][0])
    raise TemplateAssertionError(msg, -1, "", "unknown")


def jinja_warning(msg):
    """
    Reports an warning in the current jinja template.
    This can be used inside a filter to indicate problems with the passed arguments or direclty
    inside an template
    """
    file, lineno = jinjaTrace()[0]
    if file:
        message = '{0}:{1}: warning: {2}'.format(file, lineno, msg)
    else:
        message = '<unknown-file>: warning: {0}'.format(msg)
    click.secho(message, fg='yellow', err=True)


def deprecate_helper(version, removal, message):
    """
    Handle deprecation similarly to Qt deprecation rules.
    """

    def version_to_int(version_string):
        version_val = 0
        i = 0
        for ver in version_string.split('.'):
            version_val = version_val << 8 | int(ver)
            i += 1
            if i >= 3:
                break
        for _ in range(i, 3):
            version_val <<= 8
        return version_val

    deprecate_version = version_to_int(version)
    qt_version = version_to_int(builtin_config.config['VERSION'])
    if qt_version < deprecate_version:
        return

    removal_version = removal << 16
    qt_disable_version = version_to_int(
            builtin_config.config['QT_DISABLE_DEPRECATED_UP_TO'])
    if qt_disable_version >= deprecate_version or \
            qt_version >= removal_version:
        message = 'Using deprecated construct which has to be removed.' \
                  if message is None \
                  else 'Deprecated usage to be removed: ' + message
        jinja_error(message)
        return

    qt_warn_version = version_to_int(
            builtin_config.config['QT_WARN_DEPRECATED_UP_TO'])
    if qt_warn_version >= deprecate_version:
        message = 'Using deprecated construct.' if message is None \
                  else 'Deprecated usage: ' + message
        jinja_warning(message)
        return

    return


def jinja_deprecate_with_qt7_removal(version, reason=None):
    """
    Deprecate current filter or template in given Qt version and targets
    removal in Qt 7, optionally giving information about the deprecation.
    """
    deprecate_helper(version, 7, reason)
    return ''


def jinja_deprecate_with_qt8_removal(version, reason=None):
    """
    Deprecate current filter or template in given Qt version and targets
    removal in Qt 8, optionally giving information about the deprecation.
    """
    deprecate_helper(version, 8, reason)
    return ''


def register_global_functions(generator):
    generator.env.globals["error"] = jinja_error
    generator.env.globals["warning"] = jinja_warning
    generator.env.globals["deprecate_with_qt7_removal"] = \
        jinja_deprecate_with_qt7_removal
    generator.env.globals["deprecate_with_qt8_removal"] = \
        jinja_deprecate_with_qt8_removal
