#!/bin/sh

# Check if the first argument is "debug" or "-d" or "--debug"
if [ "$1" = "debug" ] || [ "$1" = "-d" ] || [ "$1" = "--debug" ]; then
    # Set the DEBUG_MODE environment variable for the 'make' command
    export DEBUG_MODE=true
    echo "Compiling in DEBUG MODE..."
else
    # Ensure DEBUG_MODE is not set for normal builds (important after export)
    unset DEBUG_MODE
    echo "Compiling in NORMAL MODE..."
fi


make clean
make
bin/flappy_bird_clone