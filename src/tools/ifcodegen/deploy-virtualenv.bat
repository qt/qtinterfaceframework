:: Copyright (C) 2021 The Qt Company Ltd.
:: Copyright (C) 2018 Pelagicore AG
:: SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

@ECHO OFF
SET SCRIPT=%~dp0
SETLOCAL ENABLEDELAYEDEXPANSION

IF %1%=="" (
    call:usage
)

SET VIRTUALENV=%1%
IF NOT EXIST %VIRTUALENV% (
    call:usage
)

IF NOT EXIST %VIRTUALENV%\Lib (
    call:usage
)

SET VIRTUALENV_LIB=%VIRTUALENV%\lib
SET LIB_FOLDER=%VIRTUALENV_LIB%
FOR %%F in (%VIRTUALENV_LIB%\python*) DO (
    IF EXIST %%~fF (
        SET LIB_FOLDER=%%~fF
        SET PYTHON_VERSION=%%~nF
    )
)

START /b /wait %VIRTUALENV%\Scripts\python.exe -c "import sys; print(sys.base_prefix)" > sys_path.txt
FOR /f "delims= usebackq" %%i in ("sys_path.txt") DO (
    SET ORIG_PREFIX=%%i
)

SET ORIG_LIB=%ORIG_PREFIX%\lib\%PYTHON_VERSION%
IF NOT EXIST "%ORIG_LIB%" (
    echo "Couldn't find python prefix folder. The virtualenv will not be fully functional."
    exit 1
)

echo "copying files from %ORIG_LIB% to %VIRTUALENV_LIB%"
FOR /f "usebackq" %%i in ("%SCRIPT%\deploy-virtualenv-files.txt") DO (
    IF EXIST "%ORIG_LIB%%%i\" (
        IF NOT EXIST "%VIRTUALENV_LIB%\%%i" mkdir "%VIRTUALENV_LIB%\%%i"
        xcopy "%ORIG_LIB%%%i" "%VIRTUALENV_LIB%\%%i" /E /Q /H /Y >NUL 2>&1
    ) else (
        xcopy "%ORIG_LIB%%%i" "%VIRTUALENV_LIB%" /H /Q /Y >NUL 2>&1
    )
)

FOR %%F in ("%ORIG_PREFIX%\python*.dll") DO (
    copy "%%F" "%VIRTUALENV%\Scripts\" /Y >NUL 2>&1
)

IF EXIST %ORIG_PREFIX%\DLLs\ (
    IF NOT EXIST %VIRTUALENV%\DLLs mkdir %VIRTUALENV%\DLLs
    xcopy "%ORIG_PREFIX%\DLLs" "%VIRTUALENV%\DLLs" /E /Q /H /Y >NUL 2>&1
)

echo "done"

EXIT /B %ERRORLEVEL%

:: Functions

:usage
echo "deploy-virtualenv.bat <virtualenv>"
EXIT 1


