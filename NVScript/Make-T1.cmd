@echo off
set OutPath=output

if exist NVScript_osm.cpp del NVScript_osm.cpp
if exist NVScript_osm.o del NVScript_osm.o
if exist NVScript.o del NVScript.o
if not exist MakeT1 del *.o
if not exist MakeT1 echo . >MakeT1
D:\CygWin\bin\bash --login -i ~/NVScript/NVScript/MakeNVScriptT1.ccd

echo.
if not exist NVScript.osm echo ERROR compiling NVScript.osm (T1)
if exist NVScript.osm echo NVScript.osm (T1) compiled successfully.

if exist NVScript.osm copy NVScript.osm "%OutPath%"
if exist NVScript.osm move NVScript.osm "C:\Games\DromEd1"

echo Copying documentation to output
if exist NVScript.html copy NVScript.html "%OutPath%"

if exist "%OutPath%\NVScript-T1.zip" del "%OutPath%\NVScript-T1.zip"
if exist %OutPath%\NVScript.osm  7z.exe a "%OutPath%\NVScript-T1.zip" "%OutPath%\NVScript.osm" "%OutPath%\NVScript.html" -tzip -mx9 >NUL