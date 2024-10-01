#!/bin/bash

# Load environment variables from .env file
export $(grep -v '^#' .env | xargs)

# Get the current path
CURRENT_PATH=$(pwd)

# Substitute the variables in the command
COMMAND="/usr/bin/wine Z:/usr/share/win32/gdbserver.exe --multi localhost:3234 Z:${CURRENT_PATH}/${OUT}/${APPLICATION_NAME_DBG}"

# Print the command for debugging (optional)
echo "Executing command: $COMMAND"

# Execute the command
eval $COMMAND
