@echo off
if exist NVScript_osm.cpp del NVScript_osm.cpp
if exist NVScript_osm.o del NVScript_osm.o
if exist NVScript.o del NVScript.o
if exist MakeShock del *.o
if exist MakeShock del MakeShock
D:\CygWin\bin\bash --login -i MakeNVScriptDebug.ccd

echo.
if not exist NVScript.osm echo ERROR compiling NVScript.osm
if exist NVScript.osm echo NVScript.osm compiled successfully.
