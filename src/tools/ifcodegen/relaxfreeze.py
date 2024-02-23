#!/usr/bin/env python3
# Copyright (C) 2024 The Qt Company Ltd.,
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
"""Make relaxed requirements out of frozen requirements and constraints."""

import argparse
import re


def trim_line(line):
    """Chop comment from end of line and trim white spaces from both sides."""
    commentpos = line.find('#')
    if commentpos >= 0:
        line = line[:commentpos]
    return line.strip()


def get_pkg_name(requirement):
    """Extract package name from a requirement line."""
    match = re.search('[ ><=~[;]', requirement)
    if match:
        return requirement[:match.start()].strip()
    return requirement.strip()


def relax_frozen(frozen_file, constraints_file, relaxed_file, kept_pkgs):
    """Process input files, relax the frozen requirements and write results."""
    if not kept_pkgs:
        kept_pkgs = []
    requirements = {}
    constraints = {}

    for line in frozen_file:
        trim = trim_line(line)
        pkg = get_pkg_name(trim)
        if pkg:
            requirements[pkg] = trim

    for line in constraints_file:
        trim = trim_line(line)
        pkg = get_pkg_name(trim)
        if pkg and pkg in requirements:
            constraints[pkg] = trim
            if pkg not in kept_pkgs:
                requirements[pkg] = trim

    to_remove = []
    for pkg in requirements:
        if pkg not in constraints and pkg not in kept_pkgs:
            to_remove.append(pkg)

    for pkg in to_remove:
        del requirements[pkg]

    for (pkg, requirement) in requirements.items():
        relaxed_file.write(f'{requirement}\n')


def main():
    """Give help on usage and set up input and output files."""
    parser = argparse.ArgumentParser(
        description='''Merges a relaxed requirements file from a frozen
            requirements file and a constraints file by putting all the
            packages found in both into the output file by using the
            constraints instead of specific version numbers. However, specific
            packages can be fixed to the versions in the frozen file.''',
        epilog='''Copyright (C) 2024 The Qt Company Ltd.''')
    parser.add_argument(
        'frozenfile',
        type=argparse.FileType('r'),
        help='''Requirements file created by pip freeze having fixed version
            specifications''')
    parser.add_argument(
        'constraintsfile',
        type=argparse.FileType('r'),
        help='''File of constraints in requirements format from which relaxed
            version requirements will be used by default''')
    parser.add_argument(
        'relaxedfile',
        type=argparse.FileType('w'),
        help='The relaxed output file created in requirements format')
    parser.add_argument(
        '--keep',
        nargs='*',
        metavar='kept packages',
        dest='keep',
        help='Package names in the frozen file to be kept as fixed versions')
    args = parser.parse_args()
    relax_frozen(args.frozenfile, args.constraintsfile, args.relaxedfile,
                 args.keep)


if __name__ == '__main__':
    main()
