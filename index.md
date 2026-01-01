---
layout: default_c
RefPages:
 - howto_create_a_dev_container
--- 

# Cross-Compiling Win32 <span style="color: #409EFF; font-size: 0.6em; font-style: italic;"> -  Docker Container</span>

![MIT License](https://img.shields.io/badge/License-MIT-green) ![Commercial Services Available](https://img.shields.io/badge/Services-Optional-blue)

## ℹ️ Introduction

This Docker template stack enables cross-compilation of Win32 GUI applications using the MinGW API. The container forwards graphical output from Linux to an X11 server (**XLaunch**) running on your Windows host. You can develop Win32 C and C++ applications and test them directly within the container using **Wine**—all with GUI output displayed on Windows.

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
    <span class="nje-column2-desc">Development environment for Win32 MINGW C development projects, includes a Win32 C template project. </span>
    <span class="nje-column3-button"> 
        <a href="./Howtos/howto_create_a_dev_container#31-creating-a-win32-c-development-sub-container-afx-x11-forward-win32-c-service">Setup Guide</a>
    </span>
</span>
<span class="nje-table-row">
    <span class="nje-column1-value">Win32 C++ Cross Compilation</span>
    <span class="nje-column2-desc">Development environment for Win32 C++ MINGW development projects, includes a Win32 C++ template project. </span>
    <span class="nje-column3-button"> 
       <a href="./Howtos/howto_create_a_dev_container#32-creating-a-win32-cpp-development-sub-container-afx-x11-forward-win32-cpp-service">Setup Guide</a>
    </span>
</span>
 </div>
 <div class="nje-br3"> </div>
<br>

> **Announcement:**{: style="color: blue;font-size:11px; "} <br>
*This container has been updated and reviewed(nov 2025) The sub-container of the stack component: **App X11 Forward GUI Cross-Compiling Win32 C++**  has been integrated into this stack component. This means that the component **App X11 Forward GUI Cross-Compiling Win32 C++** is now declared deprecated and will be removed in the future*{: style="color: Darkgray;font-size:11px; "} <br>

<span style="color: #6d757dff; font-size: 13px; font-style: italic;"> <br>
<i><b>License</b><br>This file is part of: **App-X11-Forward-win32-C/C++ Stack**  Copyright (c) 2025-2026 Nico Jan Eelhart.This repository is [MIT licensed](MIT-license.md) and free to use. For optional commercial support, customization, training, or long-term maintenance, see [COMMERCIAL.md](COMMERCIAL.md).</i>
</span>

<br>
<p align="center">─── ✦ ───</p>
