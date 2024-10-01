#!/bin/bash

# This is mega irritating! 
# This reads the `.env` file variables and adds them to the `launch.json` file as environment variables. 
# We also use the `.env` file in our Makefiles, which handle them correctly. But VSC can't
# To minimize the risk of errors from  managing two different ways to adjust the variables, we use this script.
#
#   - It use a copy of your original launch.json file name launch.json
#   - in the environment section you need to place the following placeholder and run this script
#        Sample
#       "environment": [
#            { "name": "DISPLAY", "value": ":host.docker.internal:0"},          
#              //PLACEHOLDER_6870                      
#          ],    
##
# The '.env' file includes a command that instructs the user to run this script

# While the VSC team decided not to support this (discussion 2020) others do. 
# It can be frustrating when tools have limitations like that! Here’s a summary of systems that support .env files for environment variables:
#
#    Node.js
#    Docker
#    Makefile
#    Python
#    Ruby
#    PHP
#    Go
#    Java
#    Rust
#    DotNet Core
#
# If you ever need to work around the lack of support in Visual Studio Code, you can always use your custom scripts to 
# read the .env file and set the environment variables as needed. 
#
# Ref: https://github.com/microsoft/vscode/issues/89825


cp ./.vscode/launch.json ./.vscode/launch.json.prev

# Copy the template file to launch.json
cp ./.vscode/launch.tmpl ./.vscode/launch.json

# Set the IFS to read the .env file
IFS='='

# Read each line of the .env file and prepare the environment variables
env_vars=""

while read -r line; do
    # Skip empty lines
#    [[ -z "$line" ]] && continue
    [[ -z "$line" || "$line" == \#* ]] && continue
    
    # Split the line into variable name and value
    var_name=${line%%=*}
    var_value=${line#*=}

    # Add to the environment variables string
    env_vars+="{ \"name\": \"$var_name\", \"value\": \"$var_value\" },\n\t\t\t\t\t\t"

    # Prepare for ~/.bashrc
    bashrc_vars+="export $var_name=\"$var_value\"\n"
done < .env

# Remove the trailing comma from env_vars
env_vars=${env_vars%,\n}  # Remove the trailing comma

# Replace the placeholder in launch.json with the accumulated environment variables
sed -i "s|//PLACEHOLDER_6870|$env_vars|g" ./.vscode/launch.json

# Because the environment variables in launch.json are only reference (they must exist, before they can be referenced),
# We must export them to bashrc the file and reload them, then we can reference them. THIS is altogether pretty horrible!
# Append the variables to ~/.bashrc
echo -e "$bashrc_vars" >> ~/.bashrc
source ~/.bashrc                        # make the variables available in the current session