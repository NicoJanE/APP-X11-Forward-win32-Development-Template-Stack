---
layout: default_c
RefPages:
 - howto_create_a_dev_container
--- 

<small>
<br>
_This file is part of: **App-X11-Forward-win32-Development-Template -Stack**_
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


## Quick Setup
There are no quick setup instructions for this image. First, you need to install the Base Container/Image, which takes about 20 minutes. After that, you can easily add Sub Containers. Please refer to the document [how to create a development container](./Howtos/howto_create_a_dev_container). which explains the installation of the **Base Container** and any available **Sub Containers**.



