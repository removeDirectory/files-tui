# Windows build script for FilesTUI
# Usage: .\build.ps1 [command]
# Commands: build (default), clean, rebuild, run

param(
    [string]$Command = "build"
)

$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$BuildDir = Join-Path $ProjectRoot "build"
$BinaryPath = Join-Path $BuildDir "main.exe"

function Build {
    Write-Host "Building FilesTUI..." -ForegroundColor Green
    
    if (-not (Test-Path $BuildDir)) {
        Write-Host "Creating build directory..." -ForegroundColor Yellow
        New-Item -ItemType Directory -Path $BuildDir | Out-Null
    }
    
    Push-Location $BuildDir
    
    Write-Host "Running CMake configuration..." -ForegroundColor Yellow
    cmake -G "MinGW Makefiles" `
        -DCMAKE_BUILD_TYPE=Release `
        -DCMAKE_C_COMPILER="C:/msys64/mingw64/bin/gcc.exe" `
        -DCMAKE_CXX_COMPILER="C:/msys64/mingw64/bin/g++.exe" `
        -DCMAKE_MAKE_PROGRAM="C:/msys64/mingw64/bin/mingw32-make.exe" ..
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "CMake configuration failed!" -ForegroundColor Red
        Pop-Location
        exit 1
    }
    
    Write-Host "Building project..." -ForegroundColor Yellow
    cmake --build .
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Build completed successfully!" -ForegroundColor Green
        Write-Host "Binary location: $BinaryPath" -ForegroundColor Cyan
    }
    else {
        Write-Host "Build failed!" -ForegroundColor Red
        Pop-Location
        exit 1
    }
    
    Pop-Location
}

function Clean {
    Write-Host "Cleaning build files..." -ForegroundColor Yellow
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
        Write-Host "Clean completed!" -ForegroundColor Green
    }
    else {
        Write-Host "No build directory found." -ForegroundColor Yellow
    }
}

function Rebuild {
    Clean
    Build
}

function Run {
    if (-not (Test-Path $BinaryPath)) {
        Write-Host "Binary not found. Building first..." -ForegroundColor Yellow
        Build
    }
    
    Write-Host "Running application..." -ForegroundColor Green
    & $BinaryPath
}

switch ($Command.ToLower()) {
    "build" { Build }
    "clean" { Clean }
    "rebuild" { Rebuild }
    "run" { Run }
    default {
        Write-Host "Unknown command: $Command" -ForegroundColor Red
        Write-Host "Available commands: build, clean, rebuild, run" -ForegroundColor Yellow
        exit 1
    }
}
