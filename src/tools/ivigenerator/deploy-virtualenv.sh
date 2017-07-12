#!/bin/bash

#############################################################################
##
## Copyright (C) 2017 Pelagicore AG
## Contact: https://www.qt.io/licensing/
##
## This file is part of the QtIvi module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
## Commercial License Usage
## Licensees holding valid commercial Qt Automotive Suite licenses may use
## this file in accordance with the commercial license agreement provided
## with the Software or, alternatively, in accordance with the terms
## contained in a written agreement between you and The Qt Company.  For
## licensing terms and conditions see https://www.qt.io/terms-conditions.
## For further information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 as published by the Free Software
## Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
#############################################################################

SCRIPT=$(dirname $0)
usage()
{
     echo "deploy-virtualenv.sh <virtualenv>"
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
[ ! -d "$LIB_FOLDER" ] && usage
if [[ ! -e "$LIB_FOLDER/orig-prefix.txt" ]] ; then
    echo "orig-prefix.txt doesn't exist";
    exit 1
fi

ORIG_PREFIX=$(<"$LIB_FOLDER"/orig-prefix.txt)
ORIG_LIB=$ORIG_PREFIX/lib/$PYTHON_VERSION
if [[ ! -d "$ORIG_LIB" ]] ; then
    echo "$ORIG_LIB doesn't exist"
    exit 1
fi

if [[ ! -e "$SCRIPT/deploy-virtualenv-files.txt" ]] ; then
    echo "$SCRIPT/deploy-virtualenv-files.txt doesn't exist";
    exit 1
fi

echo "copying files from $ORIG_LIB to $LIB_FOLDER"
FILES=$(<$SCRIPT/deploy-virtualenv-files.txt)
for file in ${FILES} ; do
    expand_wildcard=($ORIG_LIB/$file)
    [ ! -e "$expand_wildcard" ] && continue;
    cp -af "$ORIG_LIB"/$file "$LIB_FOLDER/"
done

if [ "$(readlink -- "$VIRTUALENV/lib64")" != "lib" ] ; then
    rm -f "$VIRTUALENV/lib64"
    cd "$VIRTUALENV"
    ln -s lib lib64
    cd -
fi

echo "done"
