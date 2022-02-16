@echo off
set OutPath=Z:\www\nvscriptbeta\thief2

call PackageSource.cmd

echo Creating archive
"7z.exe" a "output\NVScript.zip" ".\output\NVScript.osm" ".\output\NVScript.html" ".\output\source" -tzip -mx9 >NUL

REM Copy build output to output path
if exist output\NVScript.osm copy output\NVScript.osm "%OutPath%"
if exist output\NVScript.html copy output\NVScript.html "%OutPath%"
if exist output\NVScript.zip copy output\NVScript.zip "%OutPath%"

REM Copy .osm to game dirs
if exist output\NVScript.osm copy output\NVScript.osm X:\Games\DromEd
if exist output\NVScript.osm copy output\NVScript.osm X:\Games\DromEd\Mayan.osm