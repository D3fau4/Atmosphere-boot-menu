@echo off
make
echo completado
echo %cd%
pause
move /y output\boot-menu.bin payload.bin
%systemroot%\system32\timeout.exe 10

