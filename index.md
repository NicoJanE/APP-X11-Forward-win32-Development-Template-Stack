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

Below is an overview of the available containers and the documentation to setup the Containers, for the quick setup see the <a href="https://nicojane.github.io/APP-X11-Forward-win32-Development-Template-Stack/Howtos/howto_create_a_dev_container#appendix-1-quick-setup">**Quick Setup**</a>

### Required Base Container Setup

For the full documentation see the Setup Guide of the Base Container, which is followed by documentation about the sub containers listed below.
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

In addition to the base container, which is required, the actual Win32 C project template is contained in a sub container. Currently we have these sub containers:

<div class="nje-table-sub-span">
<span class="nje-table-row">
    <span class="nje-column1-value">Win32 C Cross Compilation</span>
    <span class="nje-column2-desc">Development environment for Win32 MINGW development projects, plus a Win32 C template project. </span>
    <span class="nje-column3-button"> 
        <a href="./Howtos/howto_create_a_dev_container">Setup Guide</a>
    </span>
</span>
 </div>

 <div class="nje-br3"> </div> 

<span class="nje-br3"> </span>
<sub><i> This file is part of:  **App-X11-Forward-win32-C-Development-Template -Stack**
Copyright (c) 2025 Nico Jan Eelhart. This source code is licensed under the MIT License found in the  'LICENSE.md' file in the root directory of this source tree.
</i></sub>

<p align="center">─── ✦ ───</p>
