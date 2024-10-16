#!/usr/bin/env python3
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB)
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import os
import sys
import fnmatch
import click
import logging.config
import tempfile
from pathlib import Path

from qface.generator import FileSystem, Generator
from qface.watch import monitor
from qface.utils import load_filters

import generator.builtin_config as builtin_config
import generator.global_functions as global_functions
from generator.filters import register_filters
from generator.rule_generator import CustomRuleGenerator

from functools import reduce
def deep_get(dictionary, keys, default=None):
    return reduce(lambda d, key: d.get(key, default) if isinstance(d, dict) else default, keys.split("."), dictionary)


here = Path(__file__).parent

log = logging.getLogger(__file__)

deprecatedAnnotations = {}

def validateType(srcFile, type, errorString):
    if type.is_interface:
        sys.exit("{0}: {1} of type 'interface' are not supported".format(srcFile, errorString))
    if type.is_map:
        sys.exit("{0}: {1} of type 'map' are not supported".format(srcFile, errorString))


def validateSystem(srcFile, system):
    """
    Searches for types we don't support and reports an error
    Also checks all annotations for deprecations
    """
    checkDeprecationOfAnnotation(system)
    for module in system.modules:
        checkDeprecationOfAnnotation(module)
        for interface in module.interfaces:
            checkDeprecationOfAnnotation(interface)
            for property in interface.properties:
                checkDeprecationOfAnnotation(property)
                validateType(srcFile, property.type, "Properties")
            for operation in interface.operations:
                checkDeprecationOfAnnotation(operation)
                for param in operation.parameters:
                    checkDeprecationOfAnnotation(param)
                    validateType(srcFile, param.type, "Arguments")
                validateType(srcFile, operation.type, "Return values")

            for signal in interface.signals:
                checkDeprecationOfAnnotation(signal)
                for param in signal.parameters:
                    checkDeprecationOfAnnotation(param)
                    validateType(srcFile, param.type, "Arguments")

        for struct in module.structs:
            checkDeprecationOfAnnotation(struct)
            for field in struct.fields:
                checkDeprecationOfAnnotation(field)
                validateType(srcFile, field.type, "Fields")

def checkDeprecationOfAnnotation(symbol):
    type_str = symbol.__class__.__name__
    if type_str in deprecatedAnnotations:
        anno = deprecatedAnnotations[type_str]['annotation']
        if deep_get(symbol.tags, anno):
            log.warning("warning: {0} is deprecated and will be removed in future Qt versions".format(anno))

def deprecateAnnotation(type, annotation, sinceVersion):
    deprecatedAnnotations[type] = {
        'annotation': annotation,
        'sinceVersion': sinceVersion
    }

def generate(template_search_paths, tplconfig, moduleConfig, annotations, imports, src, dst):
    log.debug('run {0} {1}'.format(src, dst))
    FileSystem.strict = True
    Generator.strict = True

    # First run without imports to know the name of the modules we want to generate
    module_names = []
    system = FileSystem.parse(src)
    for module in system.modules:
        module_names.append(module.name)

    # Second run with imports to resolve all needed type information
    all_files = imports + src
    system = FileSystem.parse(all_files)
    for annotations_file in annotations:
        log.debug('{0}'.format(annotations_file))
        if not os.path.isabs(annotations_file):
            annotations_file = Path.getcwd() / str(annotations_file)
        if not Path(annotations_file).exists():
            print('no such annotation file: {0}'.format(annotations_file))
            exit(1)
        FileSystem.merge_annotations(system, Path(annotations_file))

    srcFile = os.path.basename(src[0])
    srcBase = os.path.splitext(srcFile)[0]
    global_functions.currentSrcFile = srcFile
    ctx = {
       # To be backward compatible
       'qtASVersion': builtin_config.config["VERSION"],
       'ifcodegenVersion': builtin_config.config["VERSION"],
       'srcFile': srcFile,
       'srcBase': srcBase
    }
    search_path = [tplconfig]
    # In case tplconfig is a path, we also want to add the containing folder to the search-path
    # This makes sure that also templates referenced by path can use common templates or commmon filters
    if os.path.exists(tplconfig):
        search_path += [os.path.dirname(tplconfig)]

    search_path += template_search_paths
    generator = CustomRuleGenerator(search_path=search_path, destination=dst,
                                    context=ctx, modules=module_names)
    generator.env.keep_trailing_newline = True

    global_functions.register_global_functions(generator)
    register_filters(generator)

    # Add the current path to the module search path
    # This makes it possible to import our filters.py and friends
    # from the plugin filters
    sys.path.append(os.path.join(os.path.dirname(__file__)))

    # Add a module specific extra filter if found
    extra_filter_path = os.path.dirname(tplconfig) + '/{0}/filters.py'.format(os.path.basename(tplconfig))
    if os.path.exists(extra_filter_path):
        extra_filters = load_filters(Path(extra_filter_path))
        generator.filters = extra_filters

    validateSystem(srcFile, system)

    # Make sure the config tag is available for all our symbols
    for module in system.modules:
        module.add_tag('config')
        for val, key in moduleConfig.items():
            module.add_attribute('config', val, key)
        for interface in module.interfaces:
            interface.add_tag('config')
            for property in interface.properties:
                property.add_tag('config')
            for operation in interface.operations:
                operation.add_tag('config')
            for signal in interface.signals:
                signal.add_tag('config')
        for struct in module.structs:
            struct.add_tag('config')
            for field in struct.fields:
                field.add_tag('config')
        for enum in module.enums:
            enum.add_tag('config')
            for member in enum.members:
                member.add_tag('config')

    generator.process_rules(os.path.dirname(tplconfig) + '/{0}.yaml'.format(os.path.basename(tplconfig)), system)


def run(template_search_paths, template, moduleConfig, annotations, imports, src, dst):
    templatePath = template

    foundTemplates = {}
    for path in template_search_paths:
        for f in os.listdir(path):
            if fnmatch.fnmatch(f, '*.yaml'):
                t = os.path.splitext(f)[0]
                foundTemplates[t] = Path(path) / t

    if template in foundTemplates:
        templatePath = foundTemplates[template]

    if os.path.exists(templatePath):
        generate(template_search_paths, templatePath, moduleConfig, annotations, imports, src, dst)
    else:
        print('Invalid Template: {0}. It either needs to be one of the templates found in the template'
              'search path or an existing template folder. The following templates have been found: {1}'
              .format(templatePath, list(foundTemplates.keys())))
        exit(1)


def self_check(ctx, param, value):
    if not value or ctx.resilient_parsing:
        return
    click.echo('Running self check')

    try:
        # Parse the .config file and throw an error in case it doesn't exist or it is invalid
        builtin_config.parse(here)

        tmpDir = tempfile.TemporaryDirectory()
        tmp = Path(tmpDir.name)
        with open(tmp / "test.qface", 'w') as file:
            # Write content to the file
            file.write("""
            module org.selftest 1.0

            interface Echo {
                string stringProperty
                bool boolProperty
                TestEnum enumProperty
                TestStruct structProperty
                string echo(string msg);
            }

            enum TestEnum {
                value1,
                value2
            }

            struct TestStruct {
                int testInt
                string testString
            }
            """)

        with open(tmp / "selfcheck.yaml", 'w') as file:
            # Write content to the file
            file.write("""
            test:
                module:
                    documents:
                        - "{{srcBase|lower}}": "module.tpl"
                interface:
                    documents:
                        - 'tst_{{interface|lower}}': 'interface.tpl'
            """)

        os.mkdir(tmp / "selfcheck")
        with open(tmp / "selfcheck/module.tpl", 'w') as file:
            # Write content to the file
            file.write("""
            {{module.name}}
            """)

        with open(tmp / "selfcheck/interface.tpl", 'w') as file:
            # Write content to the file
            file.write("""
            {{interface.name}}
            """)

        run([tmp], 'selfcheck', {"module": "org.selftest", "force": True}, [], [], [tmp / "test.qface"], tmp)
        click.echo('Self check finished successfully.')
    except Exception as e:
        raise SystemExit("""
              Self check failed!

              This might be caused by a too recent python version or
              too recent python packages.
              If this happens when building the qtinterfaceframework,
              you can try installing older python packages by
              running configure again with the following option:

              -DQT_USE_MINIMAL_QFACE_PACKAGES=TRUE

              The python error was:

              {}
              """.format(e))
    ctx.exit()

@click.command()
@click.option('--reload/--no-reload', default=False, help=
    'Specifies whether the generator should keep track of the changes in the IDL file and update '
    'output on the fly (--no-reload by default).')
@click.option('--template-search-path', '-T', 'template_search_paths', multiple=True, required=True, help=
    'Adds the given path to the list of template search paths. All directories in this list are '
    'scanned for template YAML files and can be selected afterwards as a generation templates using'
    '--template option.')
@click.option('--template', '-t', multiple=False, help='The template the autogenerator should use for '
    'the generation. This can either be one of the templates found in the templates search path  '
    'or a path to a template folder. ')
@click.option('--module', help='The name of the Qt module the autogenerator is '
    'generating. This is automatically used by the qmake integration and passed directly to the '
    'qface templates.')
@click.option('--force', is_flag=True, default=False, help='Always write all output files')
@click.option('--annotations', '-A', multiple=True, help=
    'Merges the given annotation file with annotions already in the qface file and the '
    'implicit annotation file. The annotation files will be merged in the order they are passed '
    'to the generator. Providing a duplicate key in the YAML file will override the previously '
    'set value. This option can be used multiple times.')
@click.option('--import', '-I', 'imports' , multiple=True, help=
    'Adds the given path to the list of import paths. All directories in this list are '
    'scanned recursively for QFace files. The QFace files found are then used to resolve '
    'the information required when importing a module; this is similar to how C++ include '
    'paths work.')
@click.option('--selfcheck', is_flag=True, default=False, callback=self_check, expose_value=False, is_eager=True, help=
    'Runs a self check using a builtin qface file and template to verify that the generator is '
    'working correctly. ')
@click.argument('src', nargs=-1, type=click.Path(exists=True))
@click.argument('dst', nargs=1, type=click.Path(exists=True))

def app(src, dst, template_search_paths, template, reload, module, force, annotations, imports):
    """
    The QtInterfaceFramework Autogenerator (ifcodegen)

    It takes several files or directories as src and generates the code
    in the given dst directory.
    """

    # Parse the .config file and throw an error in case it doesn't exist or it is invalid
    builtin_config.parse(here)

    if reload:
        script = '{0} {1} {2}'.format(Path(__file__).abspath(), ' '.join(src), dst)
        monitor(src, script)
    else:
        moduleConfig = {
            "module": module,
            "force": force
        }
        run(template_search_paths, template, moduleConfig, annotations, imports, src, dst)

if __name__ == '__main__':
    app()
