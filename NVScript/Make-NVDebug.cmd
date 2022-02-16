@echo off
if exist NVScript_osm.cpp del NVScript_osm.cpp
if exist NVScript_osm.o del NVScript_osm.o
if exist NVScript.o del NVScript.o
if exist MakeShock del *.o
if exist MakeShock del MakeShock
if exist MakeT1 del *.o
if exist MakeT1 del MakeShock
D:\CygWin\bin\bash --login -i MakeNVDebug.ccd

echo.
if not exist NVDebug.osm echo ERROR compiling NVDebug.osm
if exist NVDebug.osm echo NVDebug.osm compiled successfully.
