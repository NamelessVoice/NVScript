@ECHO OFF
REM Packages up the source code into the source directory.
REM Requires:
REM    - Git Bash with Rsync installed (see: https://prasaz.medium.com/add-rsync-to-windows-git-bash-f42736bae1b3 )
REM    - 7-ZIP in the path (used to zip the source)

echo Copying source code
if exist output/source rm -rf output/source
if exist output/NVScript-source.zip rm -rf output/source.zip

rsync -t -r --exclude-from=.gitignore --exclude=output/source --exclude=.git . output/source

REM Create "Deploy" scripts (can be used to copy the built artefacts somewhere)
echo REM This file is run when NVScript for T2 is built successfully > output/source/Deploy-T2.cmd
echo REM This file is run when NVScript for T1 is built successfully > output/source/Deploy-T1.cmd
echo REM This file is run when NVScript for SS2 is built successfully > output/source/Deploy-SS2.cmd


rem echo Zipping source code
rem "7z.exe" a "output/NVScript-source.zip" "./output/source/*" -tzip -mx9 > NUL

echo Source code packaging completed