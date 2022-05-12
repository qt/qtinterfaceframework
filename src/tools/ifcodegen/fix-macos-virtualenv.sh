#!/bin/bash

#############################################################################
##
## Copyright (C) 2022 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtInterfaceFramework module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:COMM$
##
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## $QT_END_LICENSE$
##
##
##
##
##
##
##
##
##############################################################################

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
