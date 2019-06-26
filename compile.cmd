@echo off
make
echo completado
echo %cd%
pause
rename output\boot-menu.bin payload.bin
%systemroot%\system32\timeout.exe 10

