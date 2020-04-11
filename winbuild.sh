#!/bin/bash

set -e

SCRIPT_DIR=$(dirname $(readlink -f $0))

if [[ ! -e /.dockerenv ]]; then
    $SCRIPT_DIR/rundocker.sh $0
else
    mkdir -p $SCRIPT_DIR/mumblemove/win-build
    cd $SCRIPT_DIR/mumblemove/win-build
    /usr/lib/mxe/usr/x86_64-w64-mingw32.static/qt5/bin/qmake ../mumblemove.pro
    make -j $(nproc)
fi
