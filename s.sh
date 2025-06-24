#!/bin/sh

if [ "$1" = "debug" ] || [ "$1" = "-d" ] || [ "$1" = "--debug" ]; then
    
    export DEBUG_MODE=true
    echo "Compiling in DEBUG MODE..."
else
    unset DEBUG_MODE
    echo "Compiling in NORMAL MODE..."
fi


make clean
make
bin/flappy_bird_clone