@echo off
REM Windows build script for FilesTUI
REM Usage: build.bat [command]
REM Commands: build (default), clean, rebuild, run

setlocal enabledelayedexpansion

set "COMMAND=%1"
if "!COMMAND!"=="" set "COMMAND=build"

set "PROJECT_ROOT=%~dp0"
set "BUILD_DIR=%PROJECT_ROOT%build"
set "BINARY_PATH=%BUILD_DIR%\build\main.exe"

goto !COMMAND!

:build
echo.
echo Building FilesTUI...
echo.
if not exist "%BUILD_DIR%" (
    echo Creating build directory...
    mkdir "%BUILD_DIR%"
)

cd /d "%BUILD_DIR%"

echo Running CMake configuration...
cmake -G "MinGW Makefiles" ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_C_COMPILER="C:/msys64/mingw64/bin/gcc.exe" ^
  -DCMAKE_CXX_COMPILER="C:/msys64/mingw64/bin/g++.exe" ^
  -DCMAKE_MAKE_PROGRAM="C:/msys64/mingw64/bin/mingw32-make.exe" ..

if errorlevel 1 (
    echo.
    echo CMake configuration failed!
    exit /b 1
)

echo.
echo Building project...
cmake --build .

if errorlevel 1 (
    echo.
    echo Build failed!
    exit /b 1
) else (
    echo.
    echo Build completed successfully!
    echo Binary location: %BINARY_PATH%
)

cd /d "%PROJECT_ROOT%"
endlocal
exit /b 0

:clean
echo.
echo Cleaning build files...
if exist "%BUILD_DIR%" (
    rmdir /s /q "%BUILD_DIR%"
    echo Clean completed!
) else (
    echo No build directory found.
)
endlocal
exit /b 0

:rebuild
call :clean
call :build
endlocal
exit /b 0

:run
if not exist "%BINARY_PATH%" (
    echo Binary not found. Building first...
    call :build
)

echo.
echo Running application...
"%BINARY_PATH%"
endlocal
exit /b %errorlevel%

:unknown
echo Unknown command: %COMMAND%
echo Available commands: build, clean, rebuild, run
exit /b 1
