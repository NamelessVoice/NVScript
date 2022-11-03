@echo off
rem set OutPath=Z:\www\nvscriptbeta\shock2

rem echo Preparing documentation (using Git Bash tools)
rem sed -i -b -e "s!/\*T2Version-Start\*/!/\*T2Version-Start!g" output/NVScript.html
rem sed -i -b -e "s!/\*T2Version-End\*/!T2Version-End\*/!g" output/NVScript.html
rem sed -i -b -e "s!/\*SS2Version-Start!/\*SS2Version-Start\*/!g" output/NVScript.html
rem sed -i -b -e "s!SS2Version-End\*/!/\*SS2Version-End\*/!g" output/NVScript.html
rem 
rem call PackageSource.cmd
rem 
rem echo Creating archive
rem "7z.exe" a "output\NVScript.zip" ".\output\NVScript.osm" ".\output\NVScript.html" ".\output\source" -tzip -mx9 >NUL
rem 
rem REM Copy build output to output path
rem if exist output\NVScript.osm copy output\NVScript.osm "%OutPath%"
rem if exist output\NVScript.html copy output\NVScript.html "%OutPath%"
rem if exist output\NVScript.zip copy output\NVScript.zip "%OutPath%\NVScript-SS2.zip"
rem 
rem REM Copy .osm to game dirs
rem if exist output\NVScript.osm copy output\NVScript.osm "X:\Games\ShockEd"
