---
layout: default_c
RefPages:
 - howto_create_a_dev_container
--- 

# Cross-Compiling Win32 <span style="color: #409EFF; font-size: 0.6em; font-style: italic;"> -  Docker Container</span>

## ℹ️ Introduction

A Docker Template Stack Container for **Cross-Compiling Win32** GUI Projects.
The output of the GUI application is forwarded from the Linux container to the X11 server on the host (**XLaunch**). Win32 C applications can be created using the MinGW API. Additionally, the GUI application can be run in Linux by utilizing the **Wine** environment, and the output from ***Wine*** is forwarded to the X11 server on the Windows host.

## Setup

Below an overview of the available containers and the documenattion to setup the Conatiners, for the quick setup see the [dd](#quick-setup)

### Full documentation Setup

For the full documentation see the Setup Guide of the Base Container
<div class="nje-table-base-span">
<span class="nje-table-row">
        <span class="nje-column1-value">Base Container </span>
        <span class="nje-column2-desc">Required Base container packages for the Win32 Project </span>
        <span class="nje-column3-button"> 
            <a href="./Howtos/howto_create_a_dev_container">Full Setup Guide</a>
        </span>
 </span>
 </div>


#### Sub Container Setup
In addition to the bas container, which is requirted, the real win32 C Project template is contained in a sub project, currently we have these sub containers

<div class="nje-table-sub-span">
<span class="nje-table-row">
    <span class="nje-column1-value">Win32 C Cross Compilation</span>
    <span class="nje-column2-desc">Developement environement for Win32 MINGW developement </span>
    <span class="nje-column3-button"> 
        <a href="./Howtos/howto_create_a_dev_container">Setup Guide</a>
    </span>
</span>
 </div>


 <div class="nje-br3"> </div> 


### Quick setup
If you have previously installed this container, you can use the quick setup steps below. Otherwise please first read the [Full Setup Guide](./Howtos/howto_create_a_dev_container) document.
- In case you don't have the **WSL** container
<pre class="nje-cmd-one-line"> wsl --import Ubuntu-docker-App-X11-Win32Dev ./wsl2-distro  "install.tar.gz"  </pre>
- Create docker base container
 <pre class="nje-cmd-one-line">docker-compose -f compose_app_forward_x11_win32_base.yml up -d --build --force-recreate  --remove-orphans </pre>
 - Install C sub-container
  <pre class="nje-cmd-one-line">docker-compose -f compose_win32-c_project.yml up -d  --remove-orphans --build --force-recreate  </pre>
  - Attach the docker 'C sub container' to the WSL
  <pre class="nje-cmd-multi-line">
# Start WSL
wsl -d Ubuntu-docker-App-X11-Win32Dev  

# Attach docker
docker exec -it afx-x11-forward-win32-c-service-axf-win32-c-1 /bin/bash
# If the container cannot be found, restart the Docker app and ensure 
# WSL integration is enabled in Docker settings!
</pre>

After this you should be able to open the container in VSC and start developing, be sure to run the following command first to read the application name from the .env file and set it in the project
<pre class="nje-cmd-multi-line">
source ./set_env.sh  # From terminal in the root project dir.
                     # You may need to reload the VSC Window!
</pre>


<span class="nje-br3"> </span>
<sub><i> This file is part of:  **App-X11-Forward-win32-C-Development-Template -Stack**
Copyright (c) 2025 Nico Jan Eelhart. This source code is licensed under the MIT License found in the  'LICENSE.md' file in the root directory of this source tree.
</i></sub>

<p align="center">─── ✦ ───</p>


