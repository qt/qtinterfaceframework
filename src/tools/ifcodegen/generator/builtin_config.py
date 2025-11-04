#!/usr/bin/env python3
# Copyright (C) 2021 The Qt Company Ltd.
# Copyright (C) 2019 Luxoft Sweden AB
# Copyright (C) 2018 Pelagicore AG
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB)
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import yaml
import os
import sys

config = {}


def parse(here):
    global config
    builtin_config_path = here / '.config'
    if 'IFGENERATOR_CONFIG' in os.environ:
        builtin_config_path = os.environ['IFGENERATOR_CONFIG']
    config = yaml.load(open(builtin_config_path), Loader=yaml.SafeLoader)
    if 'VERSION' not in config or 'FEATURES' not in config:
        sys.exit("Invalid builtin config")
