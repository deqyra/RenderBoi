#!/usr/bin/bash

if [ -n "$1" ]; then
    GENERATOR=$1
else
    GENERATOR="Ninja"
fi

cmake -G $GENERATOR -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
cp compile_commands.json ..
