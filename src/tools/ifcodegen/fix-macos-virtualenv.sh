#!/bin/bash
# Copyright (C) 2021 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

SCRIPT=$(dirname $0)
usage()
{
     echo "fix-macos-virtualenv.sh <virtualenv>"
     exit 1
}

[ "$#" -lt 1 ] && usage
VIRTUALENV="${@: -1}"
[ ! -d "$VIRTUALENV" ] && usage
[ ! -d "$VIRTUALENV/lib" ] && usage
VIRTUALENV_LIB=$VIRTUALENV/lib
for file in "$VIRTUALENV_LIB"/python* ; do
    if [[ -d $file ]] ; then
        LIB_FOLDER=$file
        PYTHON_VERSION=$(basename "$file")
        break
    fi
done

if [ "$(uname)" != "Darwin" ]; then
    exit 1
fi

mkdir bk
cp $VIRTUALENV/bin/python bk
mv -f bk/python $VIRTUALENV/bin/python
rmdir bk
codesign -s - --preserve-metadata=identifier,entitlements,flags,runtime -f $VIRTUALENV/bin/python
ln -sf python $VIRTUALENV/bin/python3
ln -sf python $VIRTUALENV/bin/$PYTHON_VERSION
