#!/bin/bash

set -e

SCRIPT_DIR=$(dirname $(readlink -f $0))

OS=$1
CONFIG=$2

usage() {
    echo "Usage: $0 linux|windows [debug|release]"
    echo
    echo "Files are output in the build directory."
    exit 1
}

build() {
    local APPLICATION=$1
    local BUILD_DIR=$SCRIPT_DIR/build/$APPLICATION-$OS-$CONFIG
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    $QMAKE $QMAKE_ARGS $SCRIPT_DIR/$APPLICATION/$APPLICATION.pro
    make -j $(nproc)
}

if [[ $OS == "linux" ]]; then
    QMAKE=qmake
elif [[ $OS == "windows" ]]; then
    QMAKE=/usr/lib/mxe/usr/x86_64-w64-mingw32.static/qt5/bin/qmake
    if [[ ! -e /.dockerenv ]]; then
        echo "Entering Docker"
        $SCRIPT_DIR/rundocker.sh $0 $@
        exit $?
    fi
else
    usage
fi

if [[ $CONFIG == "release" ]]; then
    QMAKE_ARGS="CONFIG+=release DEFINES+=QT_NO_DEBUG_OUTPUT"
else
    CONFIG=debug
    if [[ $OS == "linux" ]]; then
        QMAKE_ARGS="CONFIG+=debug"
    else
        echo "Note: Not building with debug symbols for Windows"
        echo "      Need debug build of QT to do that."
    fi
fi

echo Building client
build mumblemove

echo Building server
build mumblemoveserver
