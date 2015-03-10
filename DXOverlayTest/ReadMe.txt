/*******************************************************************
* How to Build
*******************************************************************/

1)Download/Install Microsoft Visual Studio 2013
2)Download/Install Visual C++ Redistributable Packages for Visual Studio 2013 (x64 version)
3)Download/Install Microsoft DirectX SDK (June 2010)

GIT:
https://github.com/xender1/testing

In Project Properties
Configuration Properties -> VC++ Directies, make sure 
Include Directories Look like:
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include;$(VC_IncludePath);$(WindowsSDK_IncludePath);
Library Directories Look like:
C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Lib\x64;$(VC_LibraryPath_x64);$(WindowsSDK_LibraryPath_x64);

Build->Build Solution. Victory.

/*******************************************************************
* How to Run
*******************************************************************/
1) Need Windows Aero for Windows 8.1 || Enable Windows Aero
2) H1Z1.exe minimized
CURRENTLY
3) Open new Notepad and match its size to h1z1 (this is for window name matching and for the overlay to draw on) "Untitled - Notepad"
									--change this to h1z1 window name to draw overlay right on that (currently just lazy)
4) Run h1z1 and while in game start the exe (x64/Debug/*.exe)

Victory.