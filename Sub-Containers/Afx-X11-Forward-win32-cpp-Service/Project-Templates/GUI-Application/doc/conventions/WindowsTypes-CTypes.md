
### Types used: <small>(more info at \doc\WindowsTypes-Ctypes.md) </small>
For new Win32 application development, it is no longer necessary to use the old hybrid types that change based on the Unicode or ANSI definition (the so-called multibyte types). 

Instead of using types like TCHAR, LPTSTR, LPCTSTR, and similar (notice the "T" in all of them), we now prefer to use the standard C types that are appropriate for Unicode development. This means that we will always use Unicode. 

For your convenience, here is a table comparing Windows 'T' types with standard C types (preffered):

<small>

|Windows   | Standard C         | Description
|--------- | -----------------  |--------------------------------------------------------------
|TCHAR	   | wchar_t         	|Wide character (wchar_t) used for Unicode strings.
|LPTSTR	   | wchar_t*    	    |Pointer to a mutable Unicode string.
|LPCTSTR   | const wchar_t*     |Pointer to a constant Unicode string (read-only).
|LPSTR	   | char*	            |Pointer to a mutable ANSI string (only in mixed environments).
|LPCSTR	   | const char*	    |Pointer to a constant ANSI string (only in mixed environments).
|LPWSTR	   | wchar_t*	        |Pointer to a mutable Unicode string (wchar_t).
|LPCWSTR   | const wchar_t*     |Pointer to a constant Unicode string (wchar_t).

<br><br>
In some case the Win32 API system functions request for ***const char\**** ala LPCSTR (or similare type)  while we can only provide ***const wchar_t\**** (LPCTSTR) because we use Unicode. In these cases the the *const wchar_t\* can be coverted as follow:

| Wide char                         | To ANSI char                  | Use
|---------------------------------- |-------------------------------|------
| const wchar_t* (LPCTSTR)          | const LPCSTR (const char*)    | LPCSTR ConvertWCharToLPCSTR(const wchar_t* wideStr) 

<br><br>
Below are commonly used string operations, comparing Windows-style functions with their standard C counterparts:

|Windows       |  Standard C    |  Description
|----------    |-------------   |-----------------------------------------------
|TEXT("string")| L"string"	    |Macro that converts a string literal to a Unicode wide string.
|_tcslen()	   | wcslen()	    |Function to get the length of a Unicode string (wchar_t).
|_tcscpy()	   | wcscpy()	    |Function to copy one Unicode string to another.
|_tcscat()	   | wcscat()	    |Function to concatenate two Unicode strings.
|_tprintf()	   | wprintf()	    |Function to print formatted output using a Unicode string.

</small>