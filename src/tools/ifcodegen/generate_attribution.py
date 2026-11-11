#!/usr/bin/env python3
# Copyright (C) 2026 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0
"""Generate a qt_attribution.json for the third-party content shipped with ifcodegen.

With --nuitka-report, the content is taken from the Nuitka compilation report of the compiled
ifcodegen binary. Otherwise the dependencies of --package are collected from the virtualenv.

Needs to run with the python of the ifcodegen virtualenv, as the package metadata is read from
the installed distributions.
"""

import argparse
import importlib.metadata as metadata
import json
import re
import shutil
import sysconfig
import xml.etree.ElementTree as ET
from pathlib import Path

GENERATED_NOTE = ("This entry is generated from the build configuration of ifcodegen. "
                  "The exact version can differ between builds.")

LICENSE_FILE_PREFIXES = ('LICENSE', 'LICENCE', 'COPYING', 'NOTICE')


def normalize(name):
    return re.sub(r'[-_.]+', '-', name).lower()


def requirement_name(requirement):
    """Return the package name of a Requires-Dist entry, None for extras-only requirements."""
    spec, _, marker = requirement.partition(';')
    if 'extra' in marker:
        return None
    return re.split(r'[\s\[<>=!~(]', spec.strip(), maxsplit=1)[0]


def find_distribution(name):
    try:
        return metadata.distribution(name)
    except metadata.PackageNotFoundError:
        return None


def dependency_closure(package):
    """All installed distributions package depends on, without package itself."""
    found = {}
    todo = [package]
    while todo:
        name = todo.pop()
        key = normalize(name)
        if key in found:
            continue
        dist = find_distribution(name)
        if not dist:
            continue
        found[key] = dist
        for requirement in dist.requires or []:
            dependency = requirement_name(requirement)
            if dependency:
                todo.append(dependency)
    found.pop(normalize(package), None)
    return [(dist.metadata['Name'], dist.version) for dist in found.values()]


def homepage(meta):
    if meta.get('Home-page'):
        return meta['Home-page']
    urls = {}
    for entry in meta.get_all('Project-URL') or []:
        label, _, url = entry.partition(',')
        urls[label.strip().lower()] = url.strip()
    for label in ('homepage', 'source', 'source code', 'documentation'):
        if label in urls:
            return urls[label]
    return next(iter(urls.values()), '')


def license_info(meta):
    """Return the license name and, only when declared as SPDX expression, the license id."""
    expression = meta.get('License-Expression')
    if expression:
        return expression, expression
    name = (meta.get('License') or '').strip()
    if name and name != 'UNKNOWN' and '\n' not in name and len(name) < 100:
        return name, None
    for classifier in meta.get_all('Classifier') or []:
        if classifier.startswith('License ::'):
            return classifier.split('::')[-1].strip(), None
    return 'Unknown', None


def copy_license_files(dist, entry_id, output_dir):
    declared = set(dist.metadata.get_all('License-File') or [])
    result = []
    for file in dist.files or []:
        if not file.parts[0].endswith('.dist-info'):
            continue
        if file.name not in declared and not file.name.upper().startswith(LICENSE_FILE_PREFIXES):
            continue
        destination = Path('licenses', entry_id, *file.parts[1:])
        (output_dir / destination).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(dist.locate_file(file), output_dir / destination)
        result.append(destination.as_posix())
    return sorted(result)


def make_entry(entry_id, name, version, usage, qdoc_module):
    return {
        'Id': entry_id,
        'Name': name,
        'QDocModule': qdoc_module,
        'QtUsage': f'{usage} {GENERATED_NOTE}',
        'Origin': 'Provisioned',
        'SecurityCritical': False,
        'Version': version,
    }


def distribution_entry(name, version, usage, qdoc_module, output_dir):
    dist = find_distribution(name)
    entry_id = 'ifcodegen-' + normalize(name)
    entry = make_entry(entry_id, name, version, usage, qdoc_module)
    entry['DownloadLocation'] = f'https://pypi.org/project/{name}/{version}/'
    entry['PURL'] = [f'pkg:pypi/{normalize(name)}@{version}']
    if not dist:
        entry['License'] = 'Unknown'
        return entry

    meta = dist.metadata
    if meta.get('Summary'):
        entry['Description'] = meta['Summary']
    if homepage(meta):
        entry['Homepage'] = homepage(meta)
    entry['License'], license_id = license_info(meta)
    if license_id:
        entry['LicenseId'] = license_id
    license_files = copy_license_files(dist, entry_id, output_dir)
    if license_files:
        entry['LicenseFiles'] = license_files
    return entry


def python_entry(version, dlls, qdoc_module, output_dir):
    usage = ('The Python interpreter and the parts of its standard library used by ifcodegen '
             'are compiled into the ifcodegen binary.')
    if dlls:
        usage += ' The following shared libraries are bundled as well: ' + ', '.join(dlls) + '.'
    entry = make_entry('ifcodegen-python', 'Python', version, usage, qdoc_module)
    entry['Homepage'] = 'https://www.python.org/'
    entry['CPE'] = [f'cpe:2.3:a:python:python:{version}:*:*:*:*:*:*:*']
    entry['License'] = 'Python Software Foundation License Version 2'
    entry['LicenseId'] = 'PSF-2.0'
    license_file = Path(sysconfig.get_path('stdlib'), 'LICENSE.txt')
    if license_file.is_file():
        destination = Path('licenses', 'ifcodegen-python', 'LICENSE.txt')
        (output_dir / destination).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(license_file, output_dir / destination)
        entry['LicenseFiles'] = [destination.as_posix()]
    return entry


def entries_from_nuitka_report(report, package, qdoc_module, output_dir):
    root = ET.parse(report).getroot()
    entries = []
    for dist in root.iter('distribution'):
        if normalize(dist.get('name')) == normalize(package):
            continue
        entries.append(distribution_entry(dist.get('name'), dist.get('version'),
                                          'Compiled into the ifcodegen binary.',
                                          qdoc_module, output_dir))

    dlls = sorted(dll.get('name') for dll in root.iter('included_dll')
                  if dll.get('ignored') != 'yes')
    python = root.find('python')
    if python is not None:
        entries.append(python_entry(python.get('python_version'), dlls, qdoc_module, output_dir))

    entries.append(distribution_entry('Nuitka', root.get('nuitka_version'),
                                      'The Nuitka runtime and onefile bootstrap are compiled '
                                      'into the ifcodegen binary.',
                                      qdoc_module, output_dir))
    return entries


def entries_from_venv(package, qdoc_module, output_dir):
    return [distribution_entry(name, version, 'Installed into the ifcodegen virtualenv.',
                               qdoc_module, output_dir)
            for name, version in dependency_closure(package)]


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--package', required=True,
                        help='The package ifcodegen is built from. It has its own attribution '
                             'and is not listed.')
    parser.add_argument('--nuitka-report', type=Path,
                        help='The XML compilation report of the compiled ifcodegen.')
    parser.add_argument('--qdoc-module', default='qtinterfaceframework')
    parser.add_argument('--output-dir', type=Path, required=True,
                        help='Where qt_attribution.json and the license files are written.')
    args = parser.parse_args()

    shutil.rmtree(args.output_dir / 'licenses', ignore_errors=True)
    args.output_dir.mkdir(parents=True, exist_ok=True)

    if args.nuitka_report:
        entries = entries_from_nuitka_report(args.nuitka_report, args.package,
                                             args.qdoc_module, args.output_dir)
    else:
        entries = entries_from_venv(args.package, args.qdoc_module, args.output_dir)

    entries.sort(key=lambda entry: entry['Id'])
    with open(args.output_dir / 'qt_attribution.json', 'w', encoding='utf-8') as file:
        json.dump(entries, file, indent=4)
        file.write('\n')


if __name__ == '__main__':
    main()
