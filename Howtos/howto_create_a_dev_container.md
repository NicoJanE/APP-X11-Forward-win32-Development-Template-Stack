---
layout: default_c
RefPages:
 - howto_create_a_dev_container   
--- 

<!-- markdownlint-disable MD033 MD001 MD041 MD040 MD001  MD039 MD037 MD026 MD007 -->

 <div class="nje-br3"> </div> 

<div class="custom-style" style="--nje-color: #3077C8; --nje-size:12px; --nje-bg-color:#EDEDED">
**Quick links**{: style="color: #474747;font-size:12px; "} *(Reference to most common page sections)*{: style="color: #474747;font-size:11px; "}
- ***Basics***
  - [Install the WSL](#install-the-ubuntu-wsl-version)
  - [Configure the WSL](#configure-the-ubuntu-wsl-version)<br>
  - [Install the X-Server(client)](#install-the-x-server-vcxsrv)<br>
  - [Create the base container](#create-the-basic-docker-container)<br>
- ***Sub Containers***
  - [Create win32 C development container](#31-creating-a-win32-c-development-sub-container-afx-x11-forward-win32-c-service)<br>
  - [Create win32 C++ development container](#32-creating-a-win32-cpp-development-sub-container-afx-x11-forward-win32-cpp-service)<br>
  - [Attach to win32 C development container](#312-attach-to-the-win32-c-development-container)<br>
  - [Attach to win32 C++ development container](#322-attach-to-the-win32-cpp-development-container)<br>
  - [Develop with VSC](#4-develop-with-vsc-in-the-host)<br>
  - [Appendix 1 Quick Setup](#appendix-1-quick-setup) <br><br>
</div>

 <div class="nje-br2"> </div> 

# Cross-Compiling Win32 <span style="color: #409EFF; font-size: 0.6em; font-style: italic;"> -  Docker Setup & Usage Guide</span>

## ℹ️ Introduction

This is a Linux-based Docker cross-compiler environment for developing **Windows Win32 GUI applications** (C/C++) on a Windows host. The setup enables you to build, debug, and display Win32 applications within a Docker container while viewing the graphical output on your Windows desktop.

**What You'll Build:**

- A complete Win32 development environment running in Docker Desktop on Windows
- X11 forwarding setup to display GUI applications from containers on your Windows host
- Full debugging support with Visual Studio Code integration
- Support for both 32-bit and 64-bit Win32 applications using MinGW cross-compiler

**The Technical Challenge:**

Docker containers are headless (no GUI support), so we need a method to display GUI applications on the Windows host. This setup solves that by combining three key components:

1. **Docker Desktop** - Runs the Ubuntu containers with Win32 development tools (MinGW, Wine)
2. **WSL2** - Provides Docker CLI tools to control containers and routes X11 display traffic
3. **VcXsrv (XLaunch)** - X Server on Windows that receives and displays GUI output from containers

**Architecture Overview:**

<pre class="ascii-diagram">
┌─────────────────────────────────────────────────────────────────────────────────┐
│ Windows Host                                                                    │
│  ┌──────────────┐           ┌───────────────────────────────────────────┐       │
│  │ VcXsrv       │◄──────────│ WSL2                                      │       │
│  │ (X Server)   │  X11      │ - Docker CLI (docker.io pkg)              │       │
│  │              │  Display  │   with Docker Desktop daemon              │       │
│  └──────────────┘           │   → Communicates with Docker Desktop      │       │
│                             │     daemon.                               │       │
│                             │ - DISPLAY var for X11                     │       │
│                             └───────────────────────────────────────────┘       │
│                                                                 │               │
│  ┌──────────────────────────────────────────────────────────┐   │               │
│  │ Docker Desktop (Container Runtime)                       │◄──┘               │
│  │  ┌───────────────────────────────────────────────────┐   │                   │
│  │  │ Ubuntu Container                                  │   │                   │
│  │  │ - MinGW (Win32 cross-compiler)                    │   │                   │
│  │  │ - Wine (Windows emulator)                         │   │                   │
│  │  │ - Your Win32 project source code                  │   │                   │
│  │  └───────────────────────────────────────────────────┘   │                   │
│  └──────────────────────────────────────────────────────────┘                   │
└─────────────────────────────────────────────────────────────────────────────────┘
</pre>

<sup>1</sup>**Why the WSL2 Setup is Unique:**

You'll install Docker CLI tools **inside WSL2** (not to run containers there, but to communicate with Docker Desktop). This is necessary because:

- **Docker Desktop** runs the containers with the Win32 toolchain
- **WSL2** acts as a bridge - it has the Docker CLI to control containers and the `DISPLAY` variable configured to route X11 traffic to VcXsrv
- When you start containers from a specific WSL distribution, you ensure the correct `DISPLAY` configuration is used for X11 forwarding

*Important*{: style="color: orange;font-size:13px; "} <small>This container is specifically for **Win32 development**. See also our ***APP-X11-Forward-Development-Template-Stack*** which is intended for .NET development and general purpose tasks. While both could be combined, I have chosen this separation to minimize complexity and ease maintenance.</small>

**Container Structure:**

This setup uses a **Base Container** + **Sub-Container** architecture:

- **Base Container**: Provides the core Win32 cross-compilation environment (MinGW, Wine, X11 support) but no specific projects
- **Sub-Containers**: Extend the Base Container with project-specific templates, VSCode settings, and build configurations for C or C++ Win32 applications

See Section 3 for the currently supported sub-containers.

<details class="nje-warn-box">
  <summary>Network Security Considerations
  </summary>
## Security Considerations and Network Configuration  

For personal use on a developer's PC or laptop, security risks are generally lower than in a production environment. However, it's still a good idea to follow some basic security practices. When running an X server (like vcxsrv) on your Windows host, configure vcxsrv to only accept connections from the local machine. This minimizes exposure and ensures that only applications on your PC can access the server. Additionally, keep your firewall enabled and set to block unsolicited incoming connections. While this setup is for development purposes and may not require strict security measures, these simple steps can help protect your system against unexpected threats, especially when connected to less secure networks.
<br>
In practice, this means that as a developer, you should leave the XLaunch **'Extra option' -> Disable access control** ***unchecked***
</details>
<br />

---

## Project File Structure

Before starting the setup, familiarize yourself with the project's folder structure:

<sub>***Data Locations, Within root 'APP-X11-Forward-Win32-Template-Stack\'***</sub>

```
📁 APP-X11-Forward-Win32-Template-Stack/
├── 📁 Howtos/                                 # Documentation
├── 📁 Base-Container/                         # Root folder for Base Containers
│   └── 📁 Afx-Base-Win32-Service/             # The Base Win32 Container
│       ├── 📁 wsl2distro/                     # WSL Win32 environment after installation (.vhdx file)
│       ├── 📄 Dockerfile                      # Docker build configuration
│       └── 📄 docker-compose.yml              # Docker Compose configuration
└── 📁 Sub-Containers/                         # Root folder for Sub Containers
    ├── 📁 Afx-X11-Forward-win32-c-Service/    # Sub Container with Win32 GUI C desktop program
    │   ├── 📁 .vscode/                        # VSCode settings for container development
    │   ├── 📁 Project-Template/               # Template for creating desktop apps
    │   ├── 📄 .env                            # Docker Compose and Dockerfile settings
    │   ├── 📄 Dockerfile                      # Docker build configuration
    │   └── 📄 docker-compose.yml              # Docker Compose configuration
    └── 📁 Afx-X11-Forward-win32-cpp-Service/  # Sub Container with Win32 GUI C++ desktop program (todo)
```

---

<!-- <span class="nje-br"> </span> HALF height break-->

## 2 Create the Base Container

This chapter will cover the setup of the **Base Container Service** (folder: 'Base-Container') and everything else required to run a GUI application on a Windows host. At the end of this setup, we will demonstrate that the program works as expected by executing a sample X application in the Base Container (**xeyes**).

### The Basic Container Setup

Before executing the Docker Compose file, ensure that the following items are installed and configured (refer to section 1.1, Process Architecture). The steps for these items will be explained in more detail in the following paragraphs:

<span class="nje-colored-block" style="--nje-bgcolor:#06a406;; --nje-textcolor:white; --nje-vmove2:10px "> Overview</span>

- **Download the  WSL version of Ubuntu**: Obtain the special version of Ubuntu for WSL   ([Download)](https://learn.microsoft.com/en-us/windows/wsl/install-manual). Scroll to the bottom of the page for manual versions.
- **Install WSL2**: Set up a dedicated WSL2 environment to serve as the backend for the Docker container.
- **Configure the WSL Ubuntu Distribution**: Ensure that the WSL Ubuntu distribution is properly configured.
- **Install and Configure an X-Server**: Install an X-server on the Windows host; we use VcXsrv  ([Download](https://sourceforge.net/projects/vcxsrv/)) for this purpose.
- **Run Docker to Create the Basic Image**: Execute the Docker files to create the basic container image.
- **Verify the Setup**: Display the result to demonstrate that everything work

### Download the  WSL version of Ubuntu

For the rest of this installation we need the WSL Ubuntu manual installation files. Use the procedure in the ***Get the WSL Installation file*** to get the required installation file: ***'install.tar.gz'***

<details class="nje-back-box">
  <a name="manual-wsl"></a>
  <summary>Get the WSL Installation file🔧
  </summary>

### Download the WSL Import file

- From [here](https://learn.microsoft.com/en-us/windows/wsl/install-manual).
- Scroll to the **"Downloading distributions"** section.
- Download the **Ubuntu 24.04 LTS** (should result into a file called: ***'Ubuntu2404-240425.AppxBundle'***)
- Unpack and get the installation from it in the next section

#### After getting the **WSL Import** file, get the installation from it

- Rename <span class="nje-cmd-inline-sm">Ubuntu2404-240425.AppxBundle</span> to <span class="nje-cmd-inline-sm">Ubuntu2404-240425.zip</span>
- Unpack it using 7zip or similar
- Find  <span class="nje-cmd-inline-sm">Ubuntu_2404.0.5.0_x64.appx</span>
  - Rename it to: <span class="nje-cmd-inline-sm">Ubuntu_2404.0.5.0_x64.zip</span> and unpack it.
  - **Unpack Result** you'll get the file: <span class="nje-cmd-inline-sm">.\Ubuntu_2404.0.5.0_x64\install.tar.gz</span> this is what you’ll use in the next step,

<p align="center" style="padding:20px;">─── ✦ ───</p>
</details>
<div class="nje-br3"> </div>

### Install the Ubuntu WSL version

When we have the distribution source, we can install the WSL environment. To keep the Base Container files in one place we do this in the root of our Base-Service folder ( **'./Base-Container/Afx-BaseWin32-Service/wsl2distro'***).

- **Open** a Powershell CLI in the sub folder: ***'.\Base-Container\Afx-Base-Win32-Service\'***.
- **Execute** this command and replace the ***"install.tar.gz.file"*** with your location (full path)
  <pre class="nje-cmd-one-line-sm-indent1"> wsl --import Ubuntu-docker-App-X11-Win32Dev ./wsl2-distro  "install.tar.gz" </pre>  

<!-- markdownlint-disable-next-line MD022 -->
<div class="nje-expect-multi-lines-indent2">
- This creates a **Ubuntu-docker-App-X11-Win32Dev** WSL distribution in: **./wsl2-distro**. 
- **Useful WSL management commands**:
  <pre class="nje-cmd-multi-line-sm-indent1">
  # Verify installation
  wsl --list --verbose

  # Start the distribution
  wsl -d Ubuntu-docker-App-X11-Win32Dev

  # Stop the distribution  
  wsl --terminate Ubuntu-docker-App-X11-Win32Dev

  # Execute command without opening terminal
  wsl -d Ubuntu-docker-App-X11-Win32Dev -- ls /home

  # Set as default distribution
  wsl --set-default Ubuntu-docker-App-X11-Win32Dev

  # Remove the distribution (if needed)
  wsl --unregister Ubuntu-docker-App-X11-Win32Dev
  </pre>
</div>

### Configure the Ubuntu WSL version

Now we need to update and configure our distribution. Start the WSL distribution and execute the following Linux commands:

<span class="nje-colored-block" style="--nje-bgcolor:red; --nje-textcolor:yellow; ">
 Note: For each step, select and run the entire numbered section—including all comments</span>

<pre class="nje-cmd-multi-line-sm-indent0" style="margin-top:-14px;">
# 1.1                                   # Update the Ubuntu distribution
apt update && apt upgrade -y            

#           ***IMPORTANT***
# 1.2.Install Docker CLI tools in WSL
# This installs the docker command-line tools that
# will communicate with Docker Desktop's daemon on
# the Windows host (not a separate Docker engine)
# These tools are essential for the procedure
#  described in: 
#   - 'Start Docker from the WSL Distribution'
#
apt update && apt install docker.io -y

# 2.1 The next command will update our DISPLAY environment variable
export DISPLAY=$(grep -oP "(?<=nameserver ).+" /etc/resolv.conf):0

# 2.2   Display the variable (check)
echo $DISPLAY

# 3.1 Make sure the Docker daemon is automatically
# started
cat >> ~/.bashrc << 'EOF'

# Start Docker daemon if not running
if (! pgrep -x "dockerd" > /dev/null); then
    sudo dockerd &
fi
EOF

# 3.2 Set up container identification (custom prompt)
cat >> ~/.bashrc << 'EOF'
PS1="\[\033[91m\]WSL:\[\033[0m\]\[\033[0;33m\]\
${debian_chroot:+($debian_chroot)}\u\[\033[0m\]\
:\[\033[91m\] App-X11-Win32Dev \[\033[0m\]../\W# "
EOF

# 4 Reload the configuration to apply the changes
source ~/.bashrc   # Reload, enter
# check with: ps aux | grep dockerd  # should show dockerd

# Optional to logout and leave the wsl running
exit

</pre>

### Install the X-Server (VcXsrv)

To install the X-Server in the Windows host, and receive graphical output from the WSL application, follow these instructions:

- [Download]( https://sourceforge.net/projects/vcxsrv/) and Install the VcXsrv software.
- After installation start XLaunch
  - Select **Multiple Windows** and click **Next**
  - Select Start **no client** and click **Next**
  - Ensure that **Clipboard** and **Native opengl** are **enabled**'
  - Ensure that **Disable access control** is **not enabled** ( this is more secure; only enable it if you encounter issues) click **Next**, then **Finish**

### Create the basic Docker Container

Finally, to create an start the base container.

- Open the service sub folder: ***'.\Base-Container\Afx-Base-Win32-Service\\***' within a new CMD
- Make sure you are **logged in** to **Docker**
- We use a fixed IP address in the Compose file to make it easier to communicate with services, such as an SSH server (not used in this setup). While this is not strictly necessary, we have included it by default. If you encounter any issues, you may choose to remove it from the **compose_app_forward_x11_win32_base.yml** file. The pre-configured IP address used can be found in the **.env** file. see:

<pre class="nje-cmd-multi-line-sm-indent4" style="margin-top:-14px;"> FIXED_SUBNET  # Default: 172.16.0.0/16            FIXED_IP      # Default: 172.16.0.18</pre>

- Execute this PowerShell command in the ***'Afx-Base-Win32-Service sub folder'***
  <pre class="nje-cmd-one-line-sm-indent1"> docker-compose -f compose_app_forward_x11_win32_base.yml up -d --build --force-recreate  --remove-orphans </pre>

<div class="nje-expect-multi-lines-indent2">

- In Docker Desktop a container is present with the name:***afx-base-win32-service/afx-base-win32-service-1***
- In Docker Desktop a image is present with the name:  ***eelhart/appforwardx11-win32-base*** This image is used by other sub containers!

</div>
<span class="nje-br2"> </span>

<details class="nje-warn-box">
  <summary>Recreate Containers
  </summary>
  When recreating the same container(service name) avoid subtle/annoying caching issues, to avoid irritation, make sure to:
  - delete the container
  - delete the volume and 
  - use the Docker prune command,so: 
    <pre class="nje-cmd-one-line-sm-ident1"> docker system prune -a --volumes</pre>
</details>

<span class="nje-br2"> </span> 

<details class="nje-warn-box">
  <summary>Don't Delete the base image (yet)
  </summary>
  The interesting parts will be inside the sub-containers that you are going to install next. For this reason, you can delete the created container (Because the sub container adds on to the base container), but you must keep the image file, as the sub-containers will need it! 

  But at this point: **Don't delete** the base container yet. First, let's ensure everything works as expected. This will be covered in the next subsections. 

  Remove Unused containers for good with:
  <pre class="nje-cmd-one-line-sm-ident1"> docker system prune -a --volumes</pre>
</details>
<span class="nje-br"> </span> 

### Start Docker from the WSL Distribution

**The Problem:** When running multiple WSL distributions, Docker Desktop's automatic integration (Settings → Resources → WSL integration) cannot reliably control which distribution is used—Docker may randomly select any enabled distribution.

**Our Solution:** Start the Docker container directly from a specific WSL distribution to guarantee predictable behavior.

**Why Both Are Needed:**

## Prerequisites

1. **Enable WSL integration in Docker Desktop settings**
   - Go to Docker Desktop → Settings → Resources → WSL integration
   - Select your WSL distribution (e.g., `Ubuntu-docker-App-X11-Win32Dev`) and ensure only this one is enabled
   - Click Apply & Restart to activate the integration (this may restart Docker Desktop automatically)
   - This step is essential for communication between WSL and Docker Desktop

2. **Restart Docker Desktop** (the application, not just the container) if it did not restart automatically
   - Without this, you may run into trouble when attaching from WSL
   - Error symptom: ***"cannot find the container"***

3. **Ensure the container is running** in Docker Desktop (check the dashboard)

4. **Use the specific WSL startup procedure described below**
   - Always start containers from the correct WSL distribution to ensure X11 forwarding works

## Steps to Connect

Execute these commands to attach to the container from the correct WSL distribution:

<span class="nje-ident"></span> <small>(Assumes default service name and WSL distribution name from previous steps)</small>

  <pre class="nje-cmd-multi-line-sm-indent0"> 
  # Step 1: Start the specific WSL distribution using the PowerShell CLI
  # (Docker daemon inside WSL will start automatically - press Enter a few times if needed)
  wsl -d Ubuntu-docker-App-X11-Win32Dev
  
  # Step 2: From within the WSL CLI, attach to the Docker container running on the host
  # (Use 'docker ps' from the host to verify the container name if you changed it)
  docker exec -it afx-base-win32-service-axf-basic-win32-service-1 /bin/bash
  
  # Troubleshooting: If the container cannot be found, restart Docker Desktop and try again
  </pre>

   **After these commands you can**:  
    - Execute commands at the command line prompt (see: 2.1.7 Verify the Setup)  
    - Open the Docker container in **VSC**, with the correct WSL assigned (see section 4)  
  
<details class="nje-back-box">

  <summary>Docker Desktop WSL Integration: Required Setup & Workflow Choice
  </summary>

## WSL Integration Is Required

Enabling WSL integration in Docker Desktop is **always required** for this development environment. This allows Docker Desktop to communicate with your WSL distributions and containers.

## Two Workflows: Recommended vs. Automatic

**1. Recommended Workflow (for most users, especially with multiple WSLs):**
- Enable WSL integration in Docker Desktop (see steps below)
- Start containers directly from your chosen WSL distribution (using `wsl -d <distro>` and Docker CLI)
- This ensures predictable behavior and correct X11 forwarding.

**2. Automatic Selection Workflow (not recommended, only for single/default WSL setups):**
- Enable WSL integration in Docker Desktop
- Let Docker Desktop automatically select the WSL distribution when starting containers
- This can be unpredictable if multiple WSLs are enabled, as Docker may randomly select one.

**Limitation:** With multiple WSL distributions enabled, Docker Desktop may select any of them, leading to confusion or broken setups. Always prefer the recommended workflow above.

## How to Enable WSL Integration

1. Open **Docker Desktop** → **Settings** → **Resources** → **WSL integration**
2. In the **'Enable integration with additional distros:'** section:
  - If you don't see this option, click **Refetch distros**
  - Locate and select **`Ubuntu-docker-App-X11-Win32Dev`**
  - **Ensure only this distribution is selected** (disable others to avoid conflicts)
3. Click **Apply & Restart**
  - Docker Desktop should restart automatically
  - You may need to manually restart the container if it doesn't reconnect

**Troubleshooting:** If clicking "Apply & Restart" doesn't take effect, try running Docker Desktop with **Administrator rights**, then repeat the steps above.

</details>

### 2.1.7 Verify the Setup

After running the command in section 'Create the basic Docker Container' and section 'Start Docker from the WSL Distribution' we can test if the setup **succeeded**. Make sure the docker container is started from our WSL (see: [Start Docker from the WSL Distribution](start-docker-from-the-wsl-distribution) )

#### Verify the X Output

- In th Docker container CLI prompt (which you just opened via the wsl) , started from the WSL,  enter:
  <pre class="nje-cmd-one-line-sm-ident1"> xeyes</pre>
  <span class="nje-expect-block">*This should display a pair of eyes in a Window (X is working properly)*. *When you don't see it check if XLaunch is started.* </span>

#### Verify the build environment (optional)

- Use the **'nano'** command to create a new file **'hello.c'** with this content:
  <pre class="nje-cmd-multi-line-sm">  #include &lt;windows.h&gt;
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
  {
          MessageBox(NULL, "Hello, Win32!", "Win32 Program", MB_OK);
          return 0;
  }
  </pre>

- Check if we can build the program:
  <pre class="nje-cmd-multi-line-sm"># BUILD Commands:
i686-w64-mingw32-gcc hello.c -o out.exe       # Creates 32 bits App
# x86_64-w64-mingw32-gcc hello.c -o out.exe    # Creates 64 bits App
# i686-w64-mingw32-c++ hello.c++ -o out.exe    # Creates 32 bits App
# x86_64-w64-mingw32-gcc hello.c++ -o out.exe  # Creates 64 bits App </pre>
  
- And finally check if it runs
    <pre class="nje-cmd-multi-line-sm">wine ./out.exe</pre>
    <span class="nje-expect-block">*This should display a Window with 'Hello' in it (Build configured properly)* </span>

<details class="nje-warn-box">
  <summary>Can I Remove the base container?
  </summary>
  To use the Sub Containers, see next paragraph,  you may remove the container 'afx-win32-basic/axf-basic-win32-service-1' but you will need to hold on to the image **eelhart/appforwardx11-win32-base**
</details>
 <div class="nje-br3"> </div> 

## 2.2 What do we have and What's next?

After the previous build, the Base Container now includes:
<div class="nje-features-box">
- **MinGW** - Win32 cross-compiler (32-bit and 64-bit support)
- **Wine** - Windows emulator for running Win32 executables
- **X11 forwarding** - Configuration to display GUI applications on the Windows host
- **Development tools** - Basic build environment and utilities
</div>

***How It Works***  
We now have a Docker container that uses **MinGW** to provide the **Win32 API** for creating **basic Win32 desktop applications** for Windows. The created application can be executed in this Docker container using **Wine**, a Windows emulator. The X11 configuration, which is part of this setup, along with the XLaunch server on the Windows host, ensures that the executed application is displayed in a window on the Windows host.

***MinGW API Limitations***  
Be aware that **MinGW** does not cover all of the **APIs** provided by Microsoft. 

<div class="nje-features-box">
<span class="nje-colored-block" style="--nje-bgcolor:red; --nje-textcolor:white; --nje-vmove2:10px "> APIs Not Supported</span>
- Windows Driver Kit (WDK)
- Universal Windows Platform (UWP)
- WinRT
- (This list is not exhaustive)

<span class="nje-colored-block" style="--nje-bgcolor:orange; --nje-textcolor:white; --nje-vmove2:10px "> Partial Support APIs</span>

- COM and OLE
- Security APIs
- Windows Management Instrumentation
- (This list is not exhaustive)

</div>

***Possible Improvements***  
To improve support for the Win32 SDK, we can consider creating the following **sub-containers** in the future:

- A sub-container in which the Microsoft SDK is installed inside **Wine**
- A native cross-compiling sub-container environment that can leverage Windows tools while still using our base container

**What's Next:**  
Continue with **Section 3** to set up the currently available sub-containers. These extend the Base Container you just created (with MinGW and Wine) by adding project templates, VSCode configurations, and build tools for C and C++ Win32 applications.

---

## 3. Creating the Sub containers

This section provides various combinations of Dockerfiles and Compose files, which can be used to create different **sub-containers**. Before proceeding, ensure you have already created the **Base Container** as described in **Section 2** and that it functions correctly.

In the **Sub-Containers** folder, each sub-container is stored in a separate subfolder. Sub-containers typically consist of specific project templates, including Visual Studio Code settings, build tasks, and additional build tools or libraries when required. Since this environment is designed for Win32 development, the **focus** is on **C** and **C++** projects and related frameworks.

<div class="nje-br1"> </div>

### 3.1 Creating a Win32 C development sub-container <small>(afx-x11-forward-win32-c-service)</small>

With this sub-container, we will create a GUI project for a classic **Win32** application using the **C language**. Note that for **C++** we have a Sub-Container, see section 3.2 below.

By default, this sub-container creates a typical Win32 C Desktop application project. We currently provide only one template, but other specialized templates can be added. In the ***.env** file, the setting **PRJ_TYPE_USE_CUSTOM_APP=Yes** is used to install the template project. See Section 4 for the global usage of this container in Visual Studio Code (VSC). If there are any specifics regarding VSC, we will indicate them at the end of this section.
<div class="nje-br1"> </div>

#### 3.1.1 Steps to Create a win32 C development container

1. Open a Command PowerShell CLI in: ***.\Sub-Containers\Afx-X11-Forward-win32-c-Service\\***
2. **Configure the project**:
   - Open the ***.env*** file to adjust the necessary settings:
     - **Project Name**: Set the variable **PRJ_NAME_ARG** to your desired project name. This will be used for both the project name and the project directory. If omitted, the default value will be used.
     - **Network Configuration**: If needed, you can specify an alternative subnet and IP address by adjusting the variables **FIXED_SUBNET** and **FIXED_IP**.
3. Execute the Docker command to create the project:
   <pre class="nje-cmd-one-line-sm-indent1">docker-compose -f compose_win32-c_project.yml up -d  --remove-orphans --build --force-recreate </pre> 
   <span class="nje-expect">*Note: This will create a container and a C Win32 Desktop template project by default.*</span>
<div class="nje-br1"> </div> 

#### 3.1.2 Attach to the win32 C development container

After running the commands in 3.1.1, you can start the **C Win32 sub-container** in combination with the WSL. See the **side note: Start Docker via WSL** if you need help with this. Once started, you can use Visual Studio Code (VSC) to start developing the application. For help with VSC, see Section 4. Here are the steps in short to start/attach to the container:

- In an OS Terminal: Start the WSL and attach Docker in the WSL:
  <pre class="nje-cmd-multi-line-sm">

  wsl -d Ubuntu-docker-App-X11-Win32Dev

  # In the Resulting WSL terminal attach the docker container:
  #   - When it is not started you can do this here with:
  #       docker start afx-x11-forward-win32-c-service-axf-win32-c-1
  #
  docker exec -it afx-x11-forward-win32-c-service-axf-win32-c-1 /bin/bash

    # Restart Docker application if the container can not be found!

  # Use command 'pwd' to check your directory location
  # Make sure XLaunch is started on the host!
  #
  # WARNING: When the container can't be found, first try to restart the Docker service!
  </pre>

<div class="nje-expect-multi-lines-indent2">
Commands to check your installation in the Docker terminal:
<pre class="nje-cmd-multi-line-sm">

pwd                        # Should displays your project directory including source code 
source ./set_env.sh        # Set the environment variables from the file ***.env*** in ~/.bashrc
make all                   # Should generate executable out32.exe in _make_out
wine _make_out/app.exe   # Should display the application (running for the first time takes a while)
make -fmakefile64 all      # Should generate executable out64.exe in _make_out/64
win64e _make_out/64/app.exe   # Should display the application (running for the first time takes a while)

# Alternatively you may use cmake to build the app
cd _build
cmake ..                        # Configuration 32 bits (unicode) or use:
cmake .. -DUSE_64_BIT=ON        # OR Configuration 64 bits (unicode) Other build dir!
cmake --build .                 # Builds 32 or 64 Application in _build/bin
cmake --build . --target out32 -- VERBOSE=1 # Alternatively, be specific and use debug output
wine bin/out32.exe              # Execute it
wine64 bin/64/out64.exe         # or in case of 64 bits version
 </pre>
Yes, your development will typically be in Visual Studio Code or another IDE, but these CLI steps should demonstrate that everything is working correctly.
</div>

---
<div class="nje-br1"> </div>

### 3.2 Creating a Win32 CPP development sub-container <small>(afx-x11-forward-win32-cpp-service)</small>

With this sub-container, we will create a GUI project for a classic **Win32** application using the **C++ language**. Note that for **C** we have a dedicated Sub-container, see section 3.1 here above

By default, this sub-container creates a typical Win32 C++ Desktop application project. We currently provide only one template, but other specialized templates can be added. In the ***.env** file, the type: **PRJ_TYPE_USE_CUSTOM_APP=Yes** is used to install the template project. See Section 4 for the global usage of this container in Visual Studio Code (VSC). If there are any specifics regarding VSC, we will indicate them at the end of this section.
<div class="nje-br1"> </div>

#### 3.2.1 Steps to Create a win32 C++ development container

1. Open a Command PowerShell CLI in: ***.\Sub-Containers\Afx-X11-Forward-win32-cpp-Service\\***
2. **Configure the project**:
   - Optional, open the ***.env*** file to adjust the necessary settings:
     - **Project Name**: Set the variable **PRJ_NAME_ARG** to your desired project name. This will be used for both the project name and the project directory. If omitted, the default value will be used.
     - **Network Configuration**: If needed, you can specify an alternative subnet and IP address by adjusting the variables **FIXED_SUBNET** and **FIXED_IP**.
3. Execute the Docker command to create the project:
   <pre class="nje-cmd-one-line-sm-indent1">docker-compose -f compose_win32-cpp_project.yml up -d  --remove-orphans --build --force-recreate </pre>
   <span class="nje-expect">*Note: This will create a container and a C++ Win32 Desktop template project by default.*</span>
<div class="nje-br1"> </div>

#### 3.2.2 Attach to the win32 CPP development container

After running the commands in 3.2.1, you can start the **C++ Win32 sub-container** in combination with the WSL. See the **side note: Start Docker via WSL** if you need help with this. Once started, you can use Visual Studio Code (VSC) to start developing the application. For help with VSC, see Section 4. Here are the steps in short to start/attach to the container:

- In an OS Terminal: Start the WSL and attach Docker in the WSL:
  <pre class="nje-cmd-multi-line-sm">

  wsl -d Ubuntu-docker-App-X11-Win32Dev

  # In the Resulting WSL terminal attach the docker container:
  #   - When it is not started you can do this here with:
  #        docker start afx-x11-forward-win32-cpp-service-axf-win32-cpp-1
  #
  docker exec -it afx-x11-forward-win32-cpp-service-axf-win32-cpp-1 /bin/bash

    # Restart Docker application if the container can not be found!

  # Use command 'pwd' to check your directory location
  # Make sure XLaunch is started on the host!
  #
  # WARNING: When the container can't be found, first try to restart the Docker service!
  </pre>

<div class="nje-expect-multi-lines-indent2">
Commands to check your C++ installation in the Docker terminal:
<pre class="nje-cmd-multi-line-sm">

pwd                        # Should displays your project directory including source code 
source ./set_env.sh        # Set the environment variables from the file ***.env*** in ~/.bashrc
make all                   # Should generate executable out32.exe in _make_out
wine _make_out/app.exe   # Should display the application (running for the first time takes a while)
make -fmakefile64 all      # Should generate executable out64.exe in _make_out/64
win64e _make_out/64/app.exe   # Should display the application (running for the first time takes a while)

# Alternatively you may use cmake to build the app
cd _build
cmake ..                        # Configuration 32 bits (unicode) or use:
cmake .. -DUSE_64_BIT=ON        # OR Configuration 64 bits (unicode) Other build dir!
cmake --build .                 # Builds 32 or 64 Application in _build/bin
cmake --build . --target out32 -- VERBOSE=1 # Alternatively, be specific and use debug output
wine bin/out32.exe              # Execute it
wine64 bin/64/out64.exe         # or in case of 64 bits version
 </pre>
Yes, your development will typically be in Visual Studio Code or another IDE, but these CLI steps should demonstrate that everything is working correctly.
</div>
<div class="nje-br3"> </div>

<details class="nje-back-box">
  <summary>Troubleshooting: Starting Docker via WSL
  </summary>

### Troubleshooting: Starting Docker via WSL

As explained in ***'Start Docker from the WSL Distribution'*** we must use the docker container in combination with the correct WSL distribution, this is short recap explaining how to start the Docker container, for more detailed information see the mentioned section.

- In case of problems  (i.e. ***Docker complains: 'can find the container'*** )  first consider to **restart** the docker for desktop application (not just the container) on the host.
- Ensure the sub-container is started in Docker Desktop, so we can attach to it in WSL.
- Then execute these steps in the WL distribution:

<small>(this assumes that you did not change the service name in the  WSL name )</small>
<pre class="nje-cmd-multi-line-sm-indent1">
# To make sure the correct WSL is used by our Docker container:
#  
# Start the correct WSL, docker inside the WSL will started also
# so You may need to give a few returns after executing this command
wsl -d Ubuntu-docker-App-X11-Win32Dev

# From within the WSL attach to our Docker container at the host:
# if you changed the name use: 'docker ps' from the host to display the name
#   - When it is not started you can do this first with:
#       docker start YourSubContainer
#
docker exec -it  YourSubContainer /bin/bash
</pre>
*Warning:*{: style="color: red;font-size:12px;margin-top:111px; "} <small>When the container can't be found, first try to restart the Docker service!</small> 

<p align="center" style="padding:20px;">─── ✦ ───</p>
</details>
<br />
<!-- <span class="nje-br"> </span> HALF height break-->
---
<div class="nje-br1"> </div>

## 4 Develop with VSC in the host

To develop in **V**isual **S**tudio **C**ode we advise the following instructions

First ensure you have the following extensions installed locally to work with Docker containers:

- **Remote Development**.
  Attach VSC to Docker(Develop directly inside our Docker container ), Remote WSL, Develope on remote SSH, GitHub Codespaces. 
- **Docker** (Microsoft)
- **Dev Containers** (Microsoft) optional

### 4.1.1 Open the C application container in VSC (@host)

- **Make sure** Docker is attached from the WSL! See [here]( #312-attach-to-the-win32-c-application-container)
- Within Visual Studio Code,press CTRL-SHIFT-P or F1 and select (start typing) **Attach to running container...**
- Select our **afx-x11-forward-win32-c-service-afx-win32-c-1** container
- Alternatively you might click on the **Docker boot** on the left toolbar and select the container from there.  
This opens a new Window with the  container information

#### 4.1.2. Open Folder and building your app.

- Use the **VSC Explorer** and the **Open Folder** to open the remote container's folder. **Ensure** you open the correct folder so that the **.vscode** directory settings are applied properly.
  - Select Open Folder and enter: **/projects/win32_c/default_prj**. This will ensure the project is loaded along with the settings configured in the .vscode folder. (Alternatively, you can obtain the path by opening a terminal inside the Docker container. The initial folder shown by the pwd command will give you the correct path.)

<details class="nje-note-box">
  <summary> Recommend extensions
  </summary>
  For this Docker project we have a few extensions defined, please allow these in during the opening of the container, so when you see something like: ***'Do you want to install the recommended extensions from ...'*** Press on the **Install**  button, this makes sure all functionality will work as defined, see the file ***.vscode/extensions.json*** for the recommend extension list.

</details>
<span class="nje-br2"> </span>

When opening the Win32 C container and the project root folder in Visual Studio Code, a dedicated Visual Studio Code server will be installed within the container. This server provides a full Visual Studio Code environment with its own settings and extensions, which we have provided (see the side note below). Upon opening the folder for the first time, the system will detect any required extensions and may prompt you to install them. If so, follow the instructions to complete the installation. For a list of extensions, refer to the side note below.

<details class="nje-note-box">
  <summary> Container Extensions
  </summary>
  ### **Visual Studio Code** used container extensions
  Specific extensions for this container are installed. Local extensions should be disabled for this container to prevent poor performance in Visual Studio Code and avoid other side effects. Container-specific extensions are listed in the file located at: ***.devcontainer/devcontainer.json.***
  Currently, it contains:
  <div class="custom-style" style="--nje-color: #8b5e3c; --nje-size: 12px;">
  <span class="nje-ident"></span> ms-vscode.cpptools <br>
  <span class="nje-ident"></span> ms-vscode.cmake-tools <br>
  <span class="nje-ident"></span> ms-vscode.cpptools-extension-pack<br>
  <span class="nje-ident"></span> ms-vscode.makefile-tools<br>
  <span class="nje-ident"></span> xyz.local-history<br>
</div>
  
 *Remark:*{: style="color: black;font-size:13px; "}
 <small>Be aware that extensions may trigger other extensions to be included  <br></small>

In case the intellisense indicates errors in one of the project files this is probably due to the missing of an extension.
</details>
<span class="nje-br2"> </span>

Similar to the extensions, we also provide default settings for this container. These settings define the global configurations as well as the default build and run behavior. The build and run behavior is explained in the next section (4.1.3). See the side note below for details.
<br>

<details class="nje-note-box">
  <summary> VS Code Settings files
  </summary>
  
  To change the build targets and related settings, you will need to update the files in the **.vscode directory** of the project. This is why it’s essential to open the correct project directory; ensuring that the project directory is the root allows VS Code to search for a .vscode directory in the root to apply the settings for this project. The following settings files are available in .vscode:
  
  - .vscode/tasks.json: For build tasks.
  - .vscode/launch.json: For debugging configurations.
  - .vscode/settings.json: For general VS Code settings specific to your container.
  - .vscode/keybindings.json: Specific shortcuts.
  
   *Warning:*{: style="color: red;font-size:12px; "}
   <small>While the ***keybindings.json*** is included in.vscode it will not work cause the one generated in the roaming user profile(empty!) will be used instead. overwrite that with this content if needed.  <br></small>

</details>
<span class="nje-br2"> </span>

### 4.1.3 VSC Build tasks

In the menu **'Terminal -> Run Tasks...'** You can find the build task for our project, which are defined in the settings file (see side note above). All our build tasks start with the prefix: AFX: so they should also be self-explanatory.

### 4.1.4 Debug

To debug we use the **gdbserver** this must be started before you can debug the application. 

***To Start It Manually***  
In a CMD Docker shell (root), enter the following command: <span class="nje-cmd-inline-sm"> ./runsvr.sh</span>

This will automatically discover the application name and directory to use the correct executable for debugging. It reads the information from the ***.env file***. The same ***.env*** file is also used by VS Code (in ***launch.json*** actually, the command ***set_env.sh*** updates the launch file with the information from the .env file). 

 After starting the server you can set breakpoints and debug as you are used to.

---

### 4.2.1 Open the C++ application container in VSC (@host)

- **Make sure** Docker is attached from the WSL! See [here](#322-attach-to-the-win32-cpp_application-container)
- Within Visual Studio Code,press CTRL-SHIFT-P or F1 and select (start typing) **Attach to running container...**
- Select our **afx-x11-forward-win32-cpp-service-afx-win32-cpp-1** container
- Alternatively you might click on the **Docker boot** on the left toolbar and select the container from there.  
This opens a new Window with the  container information

#### 4.2.2. Open Folder and building your app.

- Use the **VSC Explorer** and the **Open Folder** to open the remote container's folder. **Ensure** you open the correct folder so that the **.vscode** directory settings are applied properly.
  - Select Open Folder and enter: **/projects/win32-c++/project_name/**. This will ensure the project is loaded along with the settings configured in the .vscode folder. (Alternatively, you can obtain the path by opening a terminal inside the Docker container. The initial folder shown by the pwd command will give you the correct path.)

<details class="nje-note-box">
  <summary> Recommend extensions
  </summary>
  For this Docker project we have a few extensions defined, please allow these in during the opening of the container, so when you see something like: ***'Do you want to install the recommended extensions from ...'*** Press on the **Install**  button, this makes sure all functionality will work as defined, see the file ***.vscode/extensions.json*** for the recommend extension list.

</details>
<span class="nje-br2"> </span>

When opening the Win32 C++ container and the project root folder in Visual Studio Code, a dedicated Visual Studio Code server will be installed within the container. This server provides a full Visual Studio Code environment with its own settings and extensions, which we have provided (see the side note below). Upon opening the folder for the first time, the system will detect any required extensions and may prompt you to install them. If so, follow the instructions to complete the installation. For a list of extensions, refer to the side note below.

<details class="nje-note-box">
  <summary> Container Extensions
  </summary>
  ### **Visual Studio Code** used container extensions
  Specific extensions for this container are installed. Local extensions should be disabled for this container to prevent poor performance in Visual Studio Code and avoid other side effects. Container-specific extensions are listed in the file located at: ***.devcontainer/devcontainer.json.***
  Currently, it contains:
  <div class="custom-style" style="--nje-color: #8b5e3c; --nje-size: 12px;">
  <span class="nje-ident"></span> ms-vscode.cpptools <br>
  <span class="nje-ident"></span> ms-vscode.cmake-tools <br>
  <span class="nje-ident"></span> ms-vscode.cpptools-extension-pack<br>
  <span class="nje-ident"></span> ms-vscode.makefile-tools<br>
  <span class="nje-ident"></span> xyz.local-history<br>
</div>
  
 *Remark:*{: style="color: black;font-size:13px; "}
 <small>Be aware that extensions may trigger other extensions to be included  <br></small>

In case the intellisense indicates errors in one of the project files this is probably due to the missing of an extension.
</details>
<span class="nje-br2"> </span>

Similar to the extensions, we also provide default settings for this container. These settings define the global configurations as well as the default build and run behavior. The build and run behavior is explained in the next section (4.2.3). See the side note below for details.
<br>

<details class="nje-note-box">
  <summary> VS Code Settings files
  </summary>
  
  To change the build targets and related settings, you will need to update the files in the **.vscode directory** of the project. This is why it’s essential to open the correct project directory; ensuring that the project directory is the root allows VS Code to search for a .vscode directory in the root to apply the settings for this project. The following settings files are available in .vscode:
  
  - .vscode/tasks.json: For build tasks.
  - .vscode/launch.json: For debugging configurations.
  - .vscode/settings.json: For general VS Code settings specific to your container.
  - .vscode/keybindings.json: Specific shortcuts.
  
   *Warning:*{: style="color: red;font-size:12px; "}
   <small>While the ***keybindings.json*** is included in.vscode it will not work cause the one generated in the roaming user profile(empty!) will be used instead. overwrite that with this content if needed.  <br></small>

</details>
<span class="nje-br2"> </span>

### 4.2.3 VSC Build tasks

In the menu **'Terminal -> Run Tasks...'** You can find the build task for our project, which are defined in the settings file (see side note above). All our build tasks start with the prefix: AFX: so they should also be self-explanatory.

### 4.2.4 Debug

To debug we use the **gdbserver** this must be started before you can debug the application. 

***To Start It Manually***  
In a CMD Docker shell (root), enter the following command: <span class="nje-cmd-inline-sm"> ./runsvr.sh</span>

This will automatically discover the application name and directory to use the correct executable for debugging. It reads the information from the ***.env file***. The same ***.env*** file is also used by VS Code (in ***launch.json*** actually, the command ***set_env.sh*** updates the launch file with the information from the .env file).

 After starting the server you can set breakpoints and debug as you are used to.

---

### 4.2  Backup

There is a simple backup script which you can use to backup the project to **shared-host** directory. In a Docker shell (root) execute: <span class="nje-cmd-inline-sm">./_backup </span>

<details class="nje-back-box">
  <summary>Appendix: Quick Setup
  </summary>

## Appendix 1. Quick setup

If you have previously installed this container, you can use the quick setup steps below. Otherwise, please first read or at least skim through this document.

### Prerequisites

- Make sure **VcXsrv** is installed and configured (see [section 'Install the X-Server (VcXsrv)'](#install-the-x-server-vcxsrv) if needed)
- Ensure you have the **install.tar.gz** file (see [Get WSL Installation file](#manual-wsl) if needed)

### Install WSL distribution

This step is **required** because the Docker containers use WSL to output the graphical data generated by programs running inside the container

- Execute a PowerShell command like [ (More info here)](#configure-the-ubuntu-wsl-version)
  <pre class="nje-cmd-one-line-sm-indent1"> wsl --import Ubuntu-docker-App-X11-Win32Dev ./wsl2-distro  "install.tar.gz"  </pre>

#### Configure WSL distribution

This **required** step completes the previous step by upgrading the WSL, installing the CLI Docker tools (to allow access to the Docker containers on the host), and setting a dedicated prompt.

- Start the WSL environment and run these steps in the Bash CLI, [(More info here)](#install-the-ubuntu-wsl-version)
  <pre class="nje-cmd-multi-line-sm">
  # Start WSL and run these commands inside it:
  wsl -d Ubuntu-docker-App-X11-Win32Dev
  
  # Update and install Docker CLI
  apt update && apt upgrade -y
  apt update && apt install docker.io -y
  
  # Configure DISPLAY variable
  export DISPLAY=$(grep -oP "(?<=nameserver ).+" /etc/resolv.conf):0
  
  # Add to bashrc for persistence
  cat >> ~/.bashrc << 'EOF'
  # Auto-start Docker daemon
  if (! pgrep -x "dockerd" > /dev/null); then
    sudo dockerd &
  fi

  # Custom prompt
  PS1="\[\033[91m\]WSL:\[\033[0m\]\[\033[0;33m\]\${debian_chroot:+($debian_chroot)}\u\[\033[0m\]:\[\033[91m\] App-X11-Win32Dev \[\033[0m\]../\W# "
  EOF
  
  # Reload configuration
  source ~/.bashrc
  exit
  </pre>

### Create the Docker Base container for the WSL

This step is **required** it makes sure the basic packages are installed in the Docker container, other sub containers will build on to this

- In a PowerShell CLI run this command [(More info here)](#create-the-basic-docker-container)
  <pre class="nje-cmd-one-line-sm-indent1">docker-compose -f compose_app_forward_x11_win32_base.yml up -d --build --force-recreate  --remove-orphans </pre>

### Install the sub-container 'C-Development'

This ***optional*** subcontainer adds a C development environment and installs the C Win32 template project to the container.

- In a PowerShell CLI run this command [()More info here)](#31-creating-a-win32-c-development-sub-container-afx-x11-forward-win32-c-service)
   <pre class="nje-cmd-one-line-sm-indent1">docker-compose -f compose_win32-c_project.yml up -d  --remove-orphans --build --force-recreate  </pre>

#### Attach the 'C-Development' Docker container to the WSL

This make sure the 'C-Development' container is started the correct way so it outputs Graphical information to the host
To make sure our Docker image uses the correct WSL execute these command [()more info here)](#312-attach-to-the-win32-c-development-container)

  <pre class="nje-cmd-multi-line-sm">
  # Start WSL from a PowerShell CLI
  wsl -d Ubuntu-docker-App-X11-Win32Dev  
  # Attach docker
  docker exec -it afx-x11-forward-win32-c-service-axf-win32-c-1 /bin/bash
  # If the container cannot be found, restart the Docker app and ensure
  # WSL integration is enabled in Docker settings!
  </pre>

### Install the sub-container 'C++-Development'

This ***optional*** sub container add a C++ development environment and install the C++ Win32 template project to the container

- In a PowerShell CLI run this command [()more info here)](#32-creating-a-win32-cpp-development-sub-container-afx-x11-forward-win32-cpp-service)
   <pre class="nje-cmd-one-line-sm-indent1">docker-compose -f compose_win32-cpp_project.yml up -d  --remove-orphans --build --force-recreate  </pre>

#### Attach the 'C++ Development' Docker container to the WSL

This make sure the 'C++ Development' container is started the correct way so it outputs Graphical information to the host
To make sure our Docker image uses the correct WSL execute these command [more info](#322-attach-to-the-win32-cpp-development-container)

  <pre class="nje-cmd-multi-line-sm">
  wsl -d Ubuntu-docker-App-X11-Win32Dev

  # In the Resulting WSL terminal attach the docker container:
  #   - When it is not started you can do this here with:
  #        docker start afx-x11-forward-win32-cpp-service-axf-win32-cpp-1
  #
  docker exec -it afx-x11-forward-win32-cpp-service-axf-win32-cpp-1 /bin/bash

    # Restart Docker application if the container can not be found!

  # Use command 'pwd' to check your directory location
  # Make sure XLaunch is started on the host!
  #
  # WARNING: When the container can't be found, first try to restart the Docker service!
  </pre>
<span class="nje-br2"> </span>

After this you should be able to open the container in VSC and start developing (see [section 4](#4-develop-with-vsc-in-the-host) for detailed VSC setup instructions)

</details>

<span class="nje-br3"> </span>
<sub><i> This file is part of:  **App-X11-Forward-win32-C-Development-Template -Stack**
Copyright (c) 2025 Nico Jan Eelhart. This source code is licensed under the MIT License found in the  'LICENSE.md' file in the root directory of this source tree.
</i></sub>

<p align="center">─── ✦ ───</p>