@echo off
set OutPath=output

if exist NVScript_osm.cpp del NVScript_osm.cpp
if exist NVScript_osm.o del NVScript_osm.o
if exist NVScript.o del NVScript.o
if exist NVScriptLib.o del NVScriptLib.o
if exist MakeShock del *.o
if exist MakeShock del MakeShock

if exist MakeT1 del *.o
if exist MakeT1 del MakeT1
D:\CygWin\bin\bash --login -i ~/NVScript/NVScript/MakeNVScript.ccd

echo.
if not exist NVScript.osm echo ERROR compiling NVScript.osm
if exist NVScript.osm echo NVScript.osm compiled successfully.


if exist NVScript.osm move NVScript.osm "%OutPath%"

echo Readying documentation
if exist NVScript.html copy NVScript.html "%OutPath%"

cd "%OutPath%"
if exist "NVScript.zip" del "NVScript.zip"
if exist NVScript.osm  "7z.exe" a "NVScript.zip" "NVScript.osm" "NVScript.html" -tzip -mx9 >NUL

cd ..