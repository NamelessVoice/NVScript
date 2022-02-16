@echo off
set OutPath=output

if exist NVScript_osm.cpp del NVScript_osm.cpp
if exist NVScript_osm.o del NVScript_osm.o
if exist NVScript.o del NVScript.o
if exist NVScript-SS2.o del NVScript-SS2.o
if exist NVScript-ThiefShock.o del NVScript-ThiefShock.o
if not exist MakeShock del *.o
if not exist MakeShock echo . >MakeShock
D:\CygWin\bin\bash --login -i ~/NVScript/NVScript/MakeNVScriptSS2.ccd

echo.
if not exist NVScript.osm echo ERROR compiling NVScript.osm (SS2)
if exist NVScript.osm echo NVScript.osm (SS2) compiled successfully.

if exist NVScript.osm copy NVScript.osm "%OutPath%"

if not exist NVDebug.osm echo ERROR compiling NVDebug.osm
if exist NVDebug.osm echo NVDebug.osm compiled successfully.

if exist NVDebug.osm copy NVDebug.osm "%OutPath%"



if not exist Citadel.osm echo ERROR compiling Citadel.osm (SS2)
if exist Citadel.osm echo Citadel.osm (SS2) compiled successfully.

if exist Citadel.osm copy Citadel.osm "%OutPath%"

if exist NVScript.html copy NVScript.html NVScript-SS2.html

..\replace.exe "NVScript-SS2.html" "/*T2Version-Start*/" "/*T2Version-Start"
..\replace.exe "NVScript-SS2.html" "/*T2Version-End*/" "T2Version-Start*/"
..\replace.exe "NVScript-SS2.html" "/*SS2Version-Start" "/*SS2Version-Start*/"
..\replace.exe "NVScript-SS2.html" "SS2Version-End*/" "/*SS2Version-End*/"

if exist NVScript-SS2.html move NVScript-SS2.html "%OutPath%\NVScript.html"

if exist "%OutPath%\NVScript-SS2.zip" del "%OutPath%\NVScript-SS2.zip"
if exist %OutPath%\NVScript.osm  7z.exe a "%OutPath%\NVScript-SS2.zip" "%OutPath%\NVScript.osm" "%OutPath%\NVScript.html" -tzip -mx9 >NUL