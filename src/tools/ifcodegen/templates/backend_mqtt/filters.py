#!/usr/bin/env python3
# Copyright (C) 2025 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

from qface.idl.domain import Interface, Property, Operation, Signal
from enum import Enum, auto

class AnnotationScope(Enum):
    LOCAL = auto()
    INTERFACE = auto()
    MODULE = auto()
    HIERARCHICAL = auto()

def annotation(symbol, group, key, default_value=None, scope=AnnotationScope.LOCAL):
    """
    Returns the value of the annotation identified by group and key.

    scope:
        - SCOPE_LOCAL:
            Only look on the symbol itself.
        - SCOPE_INTERFACE:
            Look on the symbol, then its interface.
        - SCOPE_MODULE:
            Look on the symbol, its interface, then its module.

    If the annotation is not found, default_value is returned.
    """

    def lookup(obj):
        if obj is None:
            return None
        return obj.tags.get(group, {}).get(key)

    # Always check the symbol itself first
    value = lookup(symbol)
    if value is not None:
        return value

    if scope == AnnotationScope.LOCAL:
        return default_value

    if scope in (AnnotationScope.INTERFACE, AnnotationScope.MODULE):
        if isinstance(symbol, (Property, Signal, Operation)):
            value = lookup(symbol.interface)
            if value is not None:
                return value

    if scope == AnnotationScope.MODULE:
        if isinstance(symbol, (Property, Signal, Operation)):
            value = lookup(symbol.interface.module)
            if value is not None:
                return value
        elif isinstance(symbol, Interface):
            value = lookup(symbol.module)
            if value is not None:
                return value

    return default_value

def mqtt_topic(symbol):
    """
    Returns the mqtt topic for a given symbol.
    """
    return annotation(symbol, 'config_mqtt', 'topic', symbol.name)

def mqtt_result_topic(symbol):
    """
    Returns the mqtt topic used for operation results.
    """
    return annotation(symbol, 'config_mqtt', 'result_topic', symbol.name + "_results")

def mqtt_use_topic_prefix(symbol):
    """
    Returns whether the mqtt topic for a given symbol should use the topic prefix.
    """
    return annotation(symbol, 'config_mqtt', 'use_topic_prefix', True, AnnotationScope.MODULE)

def mqtt_retain(symbol):
    """
    Returns whether the mqtt topic for a given symbol should retain messages.
    """
    return annotation(symbol, 'config_mqtt', 'retain', False, AnnotationScope.MODULE)

def mqtt_qos(symbol):
    """
    Returns the mqtt qos for the given symbol.
    """
    return annotation(symbol, 'config_mqtt', 'qos', 0, AnnotationScope.MODULE)

def mqtt_mandatory(symbol):
    """
    Returns whether the mqtt topic for a given symbol is mandatory.
    """
    return annotation(symbol, 'config_mqtt', 'mandatory', True, AnnotationScope.MODULE)

def mqtt_topic_prefix(symbol):
    """
    Returns the mqtt topic prefix for a given symbol as QString;
    """
    prefix = annotation(symbol, 'config_mqtt', 'topic_prefix', None)
    if prefix is None:
        return 'QString()'
    return 'u"{0}"_s'.format(prefix)

def mqtt_reset_on_error(symbol):
    """
    Returns whether the mqtt topic should be reset to the current value in case of a conversion error.
    """
    return annotation(symbol, 'config_mqtt', 'reset_on_error', False, AnnotationScope.MODULE)

def mqtt_clear_on_change(symbol):
    """
    Returns whether the mqtt topic should be cleared after every change.
    """
    # TODO Fix backend to also work with default: (True if isinstance(symbol, (Signal, Operation)) else False)
    return annotation(symbol, 'config_mqtt', 'clear_on_change', False, AnnotationScope.MODULE)

def mqtt_default_server(symbol):
    """
    Returns the mqtt default_server url for a given symbol.
    """
    return annotation(symbol, 'config_mqtt', 'default_server', "mqtt://localhost:1883", AnnotationScope.MODULE)

def mqtt_default_client_id(symbol):
    """
    Returns the default  mqtt client id for a given symbol.
    """
    return annotation(symbol, 'config_mqtt', 'default_client_id', None, AnnotationScope.MODULE)

filters['mqtt_topic'] = mqtt_topic
filters['mqtt_result_topic'] = mqtt_result_topic
filters['mqtt_use_topic_prefix'] = mqtt_use_topic_prefix
filters['mqtt_retain'] = mqtt_retain
filters['mqtt_qos'] = mqtt_qos
filters['mqtt_mandatory'] = mqtt_mandatory
filters['mqtt_topic_prefix'] = mqtt_topic_prefix
filters['mqtt_reset_on_error'] = mqtt_reset_on_error
filters['mqtt_clear_on_change'] = mqtt_clear_on_change
filters['mqtt_default_server'] = mqtt_default_server
filters['mqtt_default_client_id'] = mqtt_default_client_id
