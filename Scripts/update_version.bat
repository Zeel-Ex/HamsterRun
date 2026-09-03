@echo off
setlocal enabledelayedexpansion

set PROJDIR=%~1
for /f %%i in ('git -C "%PROJDIR%" rev-parse --short HEAD') do set HASH=%%i

set INIFILE=%PROJDIR%\Config\DefaultGame.ini

powershell -Command ^
  "(Get-Content '%INIFILE%') -replace '^ProjectVersion=.*', 'ProjectVersion=0.2.0-%HASH%' | Set-Content '%INIFILE%'"
