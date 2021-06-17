#!/bin/sh

export PROJECT="mwaExample"

# linux native
if [ "$1" = "linux" ]; then
    clear
    make -f Makefile.linux clean > /dev/null 2>&1
    make -f Makefile.linux
    exit 0;
fi

# emscripten
if [ "$1" = "emscripten" ]; then
    clear
    make -f Makefile.emscripten clean > /dev/null 2>&1
    make -f Makefile.emscripten
    
    exit 0;
fi

