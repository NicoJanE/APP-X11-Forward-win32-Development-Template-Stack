**Item 1:** ***Static Window callbacks*** <br> 
Callbacks used within the WndProc should be defined as static (preferred) or as free functions because they are invoked by the Win32 system. The WndProc itself is non-static, as it is called by the WndProcDispatcher.
<br>

see item 2: [Passing instance data to static members](win32-callbacks-data.md) 

*External reference:*
 - [How to Windproc as class method](https://stackoverflow.com/questions/21369256/how-to-use-wndproc-as-a-class-function)
 - [Mapping Wndproc to your class](https://www.codeproject.com/Articles/17894/Simple-Mapping-of-WndProc-to-your-Specific-Class-W)
