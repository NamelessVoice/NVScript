@echo off
REM This script packages the source code and builds a zip locally, moving OSM files into place as needed
REM Note that you need 7-zip installed and in the path to build the archive, and git-bash with rsync to package the source
REM (see comments in PackageSource.cmd for more details)

REM Set EnableDeploy to true to enable
set EnableDeploy=false

rem The path to output build artefacts (osm/html/zip)
set OutPath=output\thief2


REM Script follows
REM ========================
set release=Release\NVScript

if "%EnableDeploy%" == "false" exit

call PackageSource.cmd

echo Creating archive
"7z.exe" a "output\NVScript.zip" ".\%release%\NVScript.osm" ".\%release%\NVScript.html" ".\output\source" -tzip -mx9 >NUL

REM Copy build output to output path
mkdir %OutPath%
if exist %release%\NVScript.osm copy %release%\NVScript.osm "%OutPath%"
if exist %release%\NVScript.html copy %release%\NVScript.html "%OutPath%"
if exist output\NVScript.zip copy output\NVScript.zip "%OutPath%"

REM Copy .osm to game dirs
if exist %release%\NVScript.osm copy %release%\NVScript.osm X:\Games\DromEd
if exist %release%\NVScript.osm copy %release%\NVScript.osm X:\Games\DromEd\Mayan.osm