set path=%CD%\WinAVR\WinAVR\bin;%CD%\WinAVR\WinAVR\utils\bin;%CD%\WinAVR\pn;%path%
%CD%\WinAVR\WinAVR\utils\bin\make.exe -C %CD%\main.hex program
pause