---
layout: default_c
RefPages:
 - howto_create_a_dev_container
--- 

<small>
<br>
_This file is part of: **App-X11-Forward-win32-C-Development-Template -Stack**_
_Copyright (c) 2024 Nico Jan Eelhart_
_This source code is licensed under the MIT License found in the  'LICENSE.md' file in the root directory of this source tree._
</small>
<br><br>

## What
A Docker Template Stack Container for **Cross-Compiling Win32** GUI Projects.
The output of the GUI application is forwarded from the Linux container to the X11 server on the host (**XLaunch**). Win32 C applications can be created using the MinGW API. Additionally, the GUI application can be run in Linux by utilizing the **Wine** environment, and the output from Wine is forwarded to the X11 server on the Windows host.

While this is designed as a cross-compiling Win32 environment, nothing stops you from using it for other (GUI) application development!

**Available Sub Containers**
- Win32 C project environment



### Appendix 1. Quick setup
If you have previously installed this container, you can use the quick setup steps below. Otherwise please first read the [how to create a development container](./Howtos/howto_create_a_dev_container) document.
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

