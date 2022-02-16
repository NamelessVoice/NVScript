@echo off
set OutPath=output

if exist Citadel_osm.cpp del Citadel_osm.cpp
if exist Citadel_osm.o del Citadel_osm.o
if exist Citadel.o del Citadel.o
if not exist MakeShock del *.o
if not exist MakeShock echo . >MakeShock
D:\CygWin\bin\bash --login -i MakeCitadel.ccd

echo.

if not exist Citadel.osm echo ERROR compiling Citadel.osm (SS2)
if exist Citadel.osm echo Citadel.osm (SS2) compiled successfully.

if exist Citadel.osm copy Citadel.osm "%OutPath%"
