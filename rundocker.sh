#!/bin/bash

run() {
    local FLAGS=$1
    shift
    local CMD=$@
    docker run -v $HOME:$HOME --user 1000:1000 $FLAGS -w $PWD mxe $CMD
}

if [[ $# -eq 0 ]]; then
    run -it bash
else
    run "" $@
fi
