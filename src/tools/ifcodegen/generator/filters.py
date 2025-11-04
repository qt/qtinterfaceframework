#!/usr/bin/env python3
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB)
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import json
import inspect

from qface.idl.domain import Module, Interface, Property, Parameter, Field, Struct
from qface.helper.generic import lower_first, upper_first
from qface.helper.qtcpp import Filters

from .global_functions import jinja_error, jinja_warning
from . import builtin_config


def deprecated_filter(name=None):
    if not name:
        name = inspect.stack()[1][3]
    jinja_warning("The '{0}' filter is deprecated and will be removed in future Qt "
                  "versions".format(name))


def enum_value_to_cppliteral(value, module_name):
    value = value.strip().rsplit('.', 1)[-1]
    return '{0}{1}::{2}'.format(Filters.classPrefix, upper_first(module_name), value)


def enum_value(value, module_name):
    sub_values = value.split('|')
    sub_values = [enum_value_to_cppliteral(v, module_name) for v in sub_values]
    return "|".join(sub_values)


def default_type_value(symbol):
    """
    Find the default value for the type. Models are initialized as nullptr
    """
    prefix = namespace_prefix(symbol.module)
    t = symbol.type
    if t.is_primitive:
        if t.is_int:
            return 'int(0)'
        if t.is_bool:
            return 'bool(false)'
        if t.is_string:
            return 'QString()'
        if t.is_real:
            return 'qreal(0.0)'
        if t.is_var:
            return 'QVariant()'
    elif t.is_void:
        return ''
    elif t.is_enum:
        prefix = namespace_prefix(t.reference.module)
        module_name = t.reference.module.module_name
        value = next(iter(t.reference.members))
        return '{0}{1}::{2}'.format(prefix, upper_first(module_name), value)
    elif t.is_flag:
        prefix = namespace_prefix(t.reference.module)
        module_name = t.reference.module.module_name
        return '{0}{1}::{2}()'.format(prefix, upper_first(module_name), flag_type(symbol))
    elif symbol.type.is_list:
        nested = Filters.returnType(symbol.type.nested)
        return 'QVariantList()'.format(nested)
    elif symbol.type.is_struct:
        prefix = namespace_prefix(symbol.type.reference.module)
        return '{0}{1}()'.format(prefix, symbol.type.reference.name)
    elif symbol.type.is_model:
        return 'nullptr'
    jinja_error('default_type_value: Unknown parameter {0} of type {1}'.format(symbol, symbol.type))


def test_type_value(symbol):
    """
    Find a value different than the default value for the type. Models are initialized as nullptr
    """
    prefix = namespace_prefix(symbol.module)
    t = symbol.type
    if t.is_primitive:
        if t.is_int:
            return '111'
        if t.is_bool:
            return 'true'
        if t.is_string:
            return 'u"TEST STRING"_s'
        if t.is_real:
            return '1234.5678'
        if t.is_var:
            return 'QVariant("TEST VARIANT")'
    elif t.is_void:
        return ''
    elif t.is_enum:
        prefix = namespace_prefix(t.reference.module)
        module_name = t.reference.module.module_name
        value = list(iter(t.reference.members))[-1]
        return '{0}{1}::{2}'.format(prefix, upper_first(module_name), value)
    elif t.is_flag:
        prefix = namespace_prefix(t.reference.module)
        module_name = t.reference.module.module_name
        value = list(iter(t.reference.members))[-1]
        return '{0}{1}::{2}'.format(prefix, upper_first(module_name), value)
    elif symbol.type.is_list:
        value = test_type_value(t.nested.type)
        if not (t.nested.type.is_primitive):
            return 'QVariantList({{QVariant::fromValue({0})}})'.format(value)
        return 'QVariantList({0})'.format(value)
    elif symbol.type.is_struct:
        prefix = namespace_prefix(symbol.type.reference.module)
        values_string = ', '.join(test_type_value(e) for e in symbol.type.reference.fields)
        return '{0}{1}({2})'.format(prefix, symbol.type.reference.name, values_string)
    elif symbol.type.is_model:
        return 'new QIfPagingModel()'
    jinja_error('test_type_value: Unknown parameter {0} of type {1}'.format(symbol, symbol.type))


def default_value_helper(symbol, res):
    t = symbol.type
    if t.is_struct:
        if not (isinstance(res, dict) or isinstance(res, list)):
            jinja_error('default_value: value in annotation for symbol {0} is supposed to be a dict or list'.format(symbol.qualified_name))
        if len(res) != len(t.reference.fields):
            jinja_error('default_value: argument count in annotation for symbol {0} and number of struct fields '
                        'does not match: Expected {1} instead of {2}'.format(symbol.qualified_name, len(t.reference.fields), len(res)))
        values = []
        for idx, property in enumerate(res):
            values.append(default_value_helper(list(t.reference.fields)[idx], property))
        return '{0}({{{1}}})'.format(t.type, ', '.join(values))
    if t.is_model or t.is_list:
        if not isinstance(res, list):
            jinja_error('default_value: value in annotation for symbol {0} is supposed to be a list'.format(symbol.qualified_name))
        row_string = ''
        if t.nested.is_struct and t.is_list:
            row_string = ', '.join(('QVariant::fromValue({0})'.format(default_value_helper(t.nested, row))) for row in res)
        else:
            row_string = ', '.join(default_value_helper(t.nested, row) for row in res)
        return '{{{0}}}'.format(row_string)
    if t.is_enum or t.is_flag:
        module_name = t.reference.module.module_name
        return enum_value(res, module_name)
    # in case it's bool, Python True is sent to the C++ as "True", let's take care of that
    if t.is_bool:
        if res:
            return 'true'
        else:
            return 'false'
    if t.is_string:
        return 'u"{0}"_s'.format(res.replace("\\", "\\\\"))
    if t.is_var:
        if isinstance(res, str):
            res = 'u"{0}"_s'.format(res)
        return 'QVariant({0})'.format(res)

    return '{0}'.format(res)


def parameter_type_default(symbol):
    """
    Return the parameter declaration for properties, handle camel case module name
    """
    prefix = namespace_prefix(symbol.module)
    if symbol.type.is_enum or symbol.type.is_flag:
        prefix = namespace_prefix(symbol.type.reference.module)
        return '{0}{1}::{2} {3}={4}'.format(prefix, upper_first(symbol.type.reference.module.module_name), flag_type(symbol), symbol, default_type_value(symbol))
    if symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.name == 'string':
            return 'const QString &{0}=QString()'.format(symbol)
        if symbol.type.name == 'var':
            return 'const QVariant &{0}=QVariant()'.format(symbol)
        if symbol.type.name == 'real':
            return 'qreal {0}=qreal()'.format(symbol)
        return '{0} {1}={2}'.format(symbol.type, symbol, default_type_value(symbol))
    elif symbol.type.is_list:
        nested = return_type(symbol.type.nested)
        return 'const QVariantList &{1}=QVariantList()'.format(nested, symbol)
    elif symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_primitive:
            return '{0}VariantModel *{1}=QVariantModel'.format(prefix, symbol)
        elif nested.is_complex:
            return 'QIfPagingModel *{0}=nullptr'.format(symbol)
    else:
        prefix = namespace_prefix(symbol.type.reference.module)
        return 'const {0}{1} &{2}={0}{1}()'.format(prefix, symbol.type.reference.name, symbol)
    jinja_error('parameter_type_default: Unknown parameter {0} of type {1}'.format(symbol,
                                                                                   symbol.type))


def parameter_type(symbol):
    """
    Return the parameter declaration for properties, handle camel case module name
    """
    prefix = namespace_prefix(symbol.module)
    if symbol.type.is_enum or symbol.type.is_flag:
        prefix = namespace_prefix(symbol.type.reference.module)
        return '{0}{1}::{2} {3}'.format(prefix, upper_first(symbol.type.reference.module.module_name), flag_type(symbol), symbol)
    if symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.name == 'string':
            return 'const QString &{0}'.format(symbol)
        if symbol.type.name == 'var':
            return 'const QVariant &{0}'.format(symbol)
        if symbol.type.name == 'real':
            return 'qreal {0}'.format(symbol)
        return '{0} {1}'.format(symbol.type, symbol)
    elif symbol.type.is_list:
        nested = return_type(symbol.type.nested)
        return 'const QVariantList &{1}'.format(nested, symbol)
    elif symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_primitive:
            return '{0}VariantModel *{1}'.format(prefix, symbol)
        elif nested.is_complex:
            return 'QIfPagingModel *{0}'.format(symbol)
    else:
        prefix = namespace_prefix(symbol.type.reference.module)
        return 'const {0}{1} &{2}'.format(prefix, symbol.type.reference.name, symbol)
    jinja_error('parameter_type: Unknown parameter {0} of type {1}'.format(symbol, symbol.type))


def return_type(symbol):
    """
    Return the type declaration for properties, handle camel case module name
    """
    prefix = namespace_prefix(symbol.module)
    if symbol.type.is_enum or symbol.type.is_flag:
        prefix = namespace_prefix(symbol.type.reference.module)
        return '{0}{1}::{2}'.format(prefix,
                                          upper_first(symbol.type.reference.module.module_name),
                                          flag_type(symbol))
    if symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.name == 'string':
            return 'QString'
        if symbol.type.name == 'var':
            return 'QVariant'
        if symbol.type.name == 'real':
            return 'qreal'
        return symbol.type.name
    elif symbol.type.is_list:
        nested = return_type(symbol.type.nested)
        return 'QVariantList'.format(nested)
    elif symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_primitive:
            return '{0}VariantModel *'.format(prefix)
        elif nested.is_complex:
            return 'QIfPagingModel *'
    else:
        prefix = namespace_prefix(symbol.type.reference.module)
        return '{0}{1}'.format(prefix, symbol.type.reference.name)
    jinja_error('return_type: Unknown symbol {0} of type {1}'.format(symbol, symbol.type))


def flag_type(symbol):
    """
    Return the annotation for the flag type if available, the plural otherwise
    """
    actualType = symbol
    if symbol.type.reference:
        actualType = symbol.type.reference
    if actualType.is_flag:
        if 'config' in actualType.tags and 'type' in actualType.tags['config']:
            return actualType.tags['config']['type']
        return '{0}s'.format(actualType)
    return actualType


def getter_name(symbol):
    """
    Returns the getter name of the property
    """
    if type(symbol) is Property:
        if 'config' in symbol.tags and 'getter_name' in symbol.tags['config']:
            return symbol.tags['config']['getter_name']
    return symbol


def setter_name(symbol):
    """
    Returns the setter name of the property
    """
    if type(symbol) is Property:
        if 'config' in symbol.tags and 'setter_name' in symbol.tags['config']:
            return symbol.tags['config']['setter_name']
    return 'set' + symbol.name[0].upper() + symbol.name[1:]


def strip_QT(s):
    """
    If the given string starts with QT, stip it away.
    """
    s = str(s)
    if s.startswith('QT'):
        return s[2:]
    return s


def simulationData(module):
    data = {}
    for interface in module.interfaces:
        iData = {}
        if 'config_simulator' in interface.tags:
            iData = interface.tags['config_simulator']
        for property in interface.properties:
            if 'config_simulator' in property.tags:
                for p in ['range', 'domain', 'minimum', 'maximum', 'default', 'unsupported']:
                    if (property.tags['config_simulator'] is not None
                            and p in property.tags['config_simulator']):
                        if property.name not in iData:
                            iData[property.name] = {}
                        if p not in iData[property.name]:
                            iData[property.name][p] = {}
                        zones = iData.get('zones', None)
                        res = property.tags['config_simulator'][p]
                        if isinstance(res, dict) and zones:
                            for zone in zones:
                                if zone in res:
                                    if p == 'unsupported':
                                        iData[property.name][p][zone] = res[zone];
                                    else:
                                        iData[property.name][p][zone] = symbolToJson(res[zone], property.type)
                            # Also check the entry for the general zone (=)
                            if "=" in res:
                                if p == 'unsupported':
                                    iData[property.name][p]["="] = res["="];
                                else:
                                    iData[property.name][p]["="] = symbolToJson(res["="], property.type)
                        else:
                            if p == 'unsupported':
                                iData[property.name][p] = res
                            else:
                                iData[property.name][p] = symbolToJson(res, property.type)
        data[interface.name] = iData
    return json.dumps(data, indent='  ')


def symbolToJson(data, symbol):
    if symbol.type.is_struct:
        t = symbol.type
        if not (isinstance(data, dict) or isinstance(data, list)):
            jinja_error('simulationData: value in annotation for symbol {0} is supposed to be a dict or list'.format(symbol.qualified_name))
        if len(data) != len(t.reference.fields):
            print(len(data), len(t.reference.fields))
            jinja_error('simulationData: argument count in annotation for symbol {0} and number of struct fields '
                        'does not match: Expected {1} instead of {2}'.format(symbol.qualified_name, len(t.reference.fields), len(data)))
        newList = list(symbolToJson(property, list(t.reference.fields)[idx]) for idx, property in enumerate(data))
        return {"type": symbol.type.name, "value": newList}
    elif symbol.type.is_enum or symbol.type.is_flag:
        module_name = symbol.type.reference.module.module_name
        return {"type": "enum", "value": enum_value(data, module_name)}
    elif symbol.type.is_list or symbol.type.is_model:
        nested = symbol.type.nested
        if nested.is_complex:
            newList = []
            for value in data:
                newList.append(symbolToJson(value, nested))
            return newList
    return data


def qml_info_type(symbol):
    """
    Returns the correct type for the symbol, to be used inside the qmltype templates
    """
    prefix = Filters.classPrefix
    if symbol.type.is_enum or symbol.type.is_flag:
        return('{0}{1}::{2}'.format(prefix, upper_first(symbol.module.module_name),
               flag_type(symbol)))
    elif symbol.type.is_void or symbol.type.is_primitive:
        if symbol.type.is_real:
            return 'double'
        return symbol.type.name
    elif symbol.type.is_struct:
        return 'QVariant'
    elif symbol.type.is_list:
        return 'QVariantList'
    elif symbol.type.is_model:
        return 'QIfPagingModel'
    else:
        jinja_error('qml_info_type: Unknown symbol {0} of type {1}'.format(symbol, symbol.type))


def qml_type(symbol):
    """
    :param interface:
    :return: Returns the name of the interface for using in QML. This name is defined in the IDL
    under the "config" tag as "qml_type". This annotation is optional, if not provided, the
    interface name is used.
    """
    result = symbol.name
    if 'qml_type' in symbol.tags['config']:
        result = symbol.tags['config']['qml_type']
    elif 'qml_name' in symbol.tags['config']:
        result = symbol.tags['config']['qml_name']
    return result


def struct_includes(symbol):
    includesSet = set()
    tpl = '#include \"{0}.h\"'

    if isinstance(symbol, Struct):
        for val in symbol.fields:
            if val.type.is_struct:
                includesSet.add(tpl.format(val.type.reference.name).lower())
    elif isinstance(symbol, Interface):
        for val in symbol.properties:
            if val.type.is_struct:
                includesSet.add(tpl.format(val.type.reference.name).lower())
        for op in symbol.operations:
            for param in op.parameters:
                if param.type.is_struct:
                    includesSet.add(tpl.format(param.type.reference.name).lower())
            if op.type.is_struct:
                includesSet.add(tpl.format(op.type.reference.name).lower())
        for op in symbol.signals:
            for param in op.parameters:
                if param.type.is_struct:
                    includesSet.add(tpl.format(param.type.reference.name).lower())

    return includesSet


def comment_text(comment):
    """
    Returns the text of the passed comment without the leading/trailing comment tokens e.g. /**, *
    """
    comment_start = ['/**', '/*!', '/*']
    processed = []
    isComment = False

    # No comment is NOT a error
    if len(comment) == 0:
        return processed

    for token in comment_start:
        if (comment.startswith(token)):
            isComment = True
            break
    if isComment:
        comment = comment[3:-2]
    else:
        jinja_error("comment_text: The provided comment needs to be start with one of these "
                    "strings: {}".format(comment_start))

    for line in comment.splitlines():
        line = line.lstrip(" *")
        processed.append(line)
    return processed

def add_namespace_prefix(symbol, module = None):
    """
    Appends the string retrieved from \l namespace_prefix to the provided symbol.
    """
    if isinstance(symbol, Module):
        module = symbol
    elif not isinstance(symbol, str):
        module = symbol.module

    return namespace_prefix(module) + str(symbol)

def namespace_prefix(symbol):
    """
    Reads the namespace from the annotation of the module the symbol is part of and transforms this
    into a C++ prefix, which can be easily appended to a C++ class name.
    """
    if not isinstance(symbol, Module):
        jinja_error("symbol is not a module")

    ns = namespace(symbol);

    if len(ns):
        return "::" + ns + "::"
    return ""

def namespace(symbol):
    """
    Returns the namespace from the annotation of the module the symbol is part of.
    """
    if not isinstance(symbol, Module):
        jinja_error("symbol is not a module")

    if not 'config' in symbol.tags or not 'namespace' in symbol.tags['config']:
        return ""
    elif symbol.tags["config"]["namespace"] == 'qt':
        # The raw Qt namespace is not available outside of C++
        return ''
    elif symbol.tags["config"]["namespace"] == 'module':
        return str(symbol).replace('.', '::')
    elif len(symbol.tags["config"]["namespace"]):
        return symbol.tags["config"]["namespace"]
    else:
        return ""

def begin_namespace(symbol):
    """
    Returns a C++ statement to start a new namespace.
    """
    if not isinstance(symbol, Module):
        jinja_error("symbol is not a module")

    if not 'config' in symbol.tags or not 'namespace' in symbol.tags['config']:
        return ""
    elif symbol.tags["config"]["namespace"] == 'qt':
        return 'QT_BEGIN_NAMESPACE'
    elif symbol.tags["config"]["namespace"] == 'module':
        return 'namespace {0} {{'.format(str(symbol).replace('.', '::'))
    elif len(symbol.tags["config"]["namespace"]):
        return 'namespace {0} {{'.format(symbol.tags["config"]["namespace"])
    else:
        return ""

def end_namespace(symbol):
    """
    Returns a C++ statement to end a namespace.
    """
    if not isinstance(symbol, Module):
        jinja_error("symbol is not a module")

    if not 'config' in symbol.tags or not 'namespace' in symbol.tags['config']:
        return ""
    elif symbol.tags["config"]["namespace"] == 'qt':
        return 'QT_END_NAMESPACE'
    elif symbol.tags["config"]["namespace"] == 'module':
        return '}}; // namespace {0}'.format(str(symbol).replace('.', '::'))
    if len(symbol.tags["config"]["namespace"]):
        return '}}; // namespace {0}'.format(symbol.tags["config"]["namespace"])
    else:
        return ""


def register_filters(generator):
    generator.env.keep_trailing_newline = True
    generator.register_filter('return_type', return_type)
    generator.register_filter('parameter_type_default', parameter_type_default)
    generator.register_filter('parameter_type', parameter_type)
    generator.register_filter('getter_name', getter_name)
    generator.register_filter('setter_name', setter_name)
    generator.register_filter('test_type_value', test_type_value)
    generator.register_filter('default_type_value', default_type_value)
    generator.register_filter('flag_type', flag_type)
    generator.register_filter('strip_QT', strip_QT)
    generator.register_filter("enum_value", enum_value)
    generator.register_filter('simulationData', simulationData)
    generator.register_filter('qml_info_type', qml_info_type)
    generator.register_filter('qml_type', qml_type)
    generator.register_filter('struct_includes', struct_includes)
    generator.register_filter('comment_text', comment_text)
    generator.register_filter('begin_namespace', begin_namespace)
    generator.register_filter('end_namespace', end_namespace)
    generator.register_filter('namespace', namespace)
    generator.register_filter('namespace_prefix', namespace_prefix)
    generator.register_filter('add_namespace_prefix', add_namespace_prefix)
