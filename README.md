

# What
A Docker Template Stack Container for **Cross-Compiling Win32** GUI Projects.
The output of the GUI application is forwarded from the Linux container to the X11 server on the host (**XLaunch**). Win32 C applications can be created using the MinGW API. Additionally, the GUI application can be run in Linux by utilizing the **Wine** environment, and the output from Wine is forwarded to the X11 server on the Windows host.

While this is designed as a cross-compiling Win32 environment, nothing stops you from using it for other (GUI) application development!

**Available Sub Containers**
- Win32 C project environment


For more information, visit the [main documentation page](https://nicojane.github.io/APP-X11-Forward-Development-Template-Stack/). You can find other Docker Template Stack (DTS) containers  [here.](https://nicojane.github.io/Docker-Template-Stacks-Home/)

<sub> &nbsp;&nbsp;&nbsp;&nbsp; *Is this a local repository project? If so, use this local link to access the [main page](./index) file. <sub>
<br><br>


<details closed>  
  <summary class="clickable-summary">
  <span  class="summary-icon"></span> 
  Side note: Preview Markdown Files(.md)
  </summary> 	<!-- On same line is failure, Don't indent the following Markdown lines!  -->

> <br>
> 
> ### Preview Markdown Files(.md)
>
>To preview the Markdown (.md) files in this project, one of the best solutions is to open these files in Visual Studio Code (VSC) and install the plugin: **Markdown Preview GitHub Styling** (Tested with version 2.04). Other plugins, or plugins for other programs, may not always work correctly with the file links in the documentation. I use the file link syntax supported by GitHub (Jekyll), which is also compatible with the above-mentioned plugin.
>
> To display the Preview screen in VSC: 
>- Ensure that you are **not** working in ***Restricted mode***.
>- Click on the "file.md" tab and choose: "Open preview." 
>- Alternatively, you can click the 'Open Preview to the Side' button at the top right. 
>
><br>
<a href="https://github.com/mjbvz/vscode-github-markdown-preview-style" target="_blank">Click here for more information on the Markdown Preview GitHub Styling plugin</a>
</details>


<br><br>
<small>Version: 0.9 </small>