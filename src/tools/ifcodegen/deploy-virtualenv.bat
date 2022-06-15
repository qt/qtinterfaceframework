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

IF EXIST %LIB_FOLDER%\orig-prefix.txt (
    SET /p ORIG_PREFIX=<%LIB_FOLDER%\orig-prefix.txt
) else (
    IF EXIST "%VIRTUALENV%\pyvenv.cfg" (
        FOR /f "tokens=1,2 delims==" %%a in (%VIRTUALENV%\pyvenv.cfg) DO (
            SET NAME=%%a
            SET NAME=!NAME:~0,-1!
            IF !NAME!==base-prefix (
                SET ORIG_PREFIX=%%b
                SET ORIG_PREFIX=!ORIG_PREFIX:~1!
            )
        )
    ) ELSE (
        echo "Neither %LIB_FOLDER%\orig-prefix.txt nor %VIRTUALENV%\pyvenv.cfg exists"
        exit 1
    )
)

SET ORIG_LIB=%ORIG_PREFIX%\lib\%PYTHON_VERSION%
IF NOT EXIST "%ORIG_LIB%" (
    echo "%ORIG_LIB% doesn't exist"
    exit 1
)

echo "copying files from %ORIG_LIB% to %VIRTUALENV_LIB%"
FOR /f %%i in (%SCRIPT%\deploy-virtualenv-files.txt) DO (
    IF EXIST "%ORIG_LIB%%%i\" (
        IF NOT EXIST %VIRTUALENV_LIB%\%%i mkdir %VIRTUALENV_LIB%\%%i
        xcopy "%ORIG_LIB%%%i" "%VIRTUALENV_LIB%\%%i" /E /Q /H /Y >NUL 2>&1
    ) else (
        xcopy "%ORIG_LIB%%%i" "%VIRTUALENV_LIB%" /H /Q /Y >NUL 2>&1
    )
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


