#!/usr/bin/env python3
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import json
import inspect

from qface.idl.domain import Module, Interface, Property, Parameter, Field, Struct
from qface.helper.generic import lower_first, upper_first
from qface.helper.qtcpp import Filters

from generator.global_functions import jinja_error, jinja_warning
from generator.filters import deprecated_filter

def custom_filter(s):
    jinja_warning("Test calling a function from the ifcodegen within our own filters")
    return

filters['custom_filter'] = custom_filter
