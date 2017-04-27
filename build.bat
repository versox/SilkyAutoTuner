@echo off
cd src
:start
g++ main.cpp AutoFile.cpp fio\parser.cpp -I fio
pause
cls
goto start