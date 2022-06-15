#!/usr/bin/env python3
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

try:
    import generate
except Exception as e:
    raise SystemExit("""
          Verifying the generator failed!

          This might be caused by a too recent python version or
          too recent python packages. You can try installing older
          python packages by running configure again with the the
          following option:

          -DQT_USE_MINIMAL_QFACE_PACKAGES=TRUE

          The python error was:

          {}
          """.format(e))
