# Build Scripts for FilesTUI

This project includes build scripts for Windows to easily compile and run the application.

## Prerequisites

- **MSYS2 or MSYS64** - Unix-like environment with MinGW toolchain
- **CMake** (version 3.20 or later)
- **GCC/MinGW C compiler**
- **Curses Library** (ncurses or PDCurses)

### Installing MSYS64 and Dependencies

1. **Download and install MSYS2/MSYS64** from https://www.msys2.org/
   - Default install location: `C:\msys64`

2. **Update MSYS64** (run in MSYS2 terminal):
```bash
pacman -Syu
# When prompted, close the terminal and run again
pacman -Syu
```

3. **Install required packages** (run in MSYS2 terminal):
```bash
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-ncurses
```

4. **Verify installation** - In PowerShell or Command Prompt:
```powershell
C:\msys64\mingw64\bin\gcc.exe --version
C:\msys64\mingw64\bin\cmake.exe --version
```

If these commands fail, MSYS64 wasn't properly installed. If successful, you can run the build scripts.

## Using the Build Scripts

### PowerShell Script (Recommended)

```powershell
# Build the project
.\build.ps1 build

# Clean build files
.\build.ps1 clean

# Clean and rebuild
.\build.ps1 rebuild

# Build and run the application
.\build.ps1 run

# You may need to allow script execution (run once)
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### Batch Script

```cmd
# Build the project
build.bat build

# Clean build files
build.bat clean

# Clean and rebuild
build.bat rebuild

# Build and run the application
build.bat run
```

## Build Output

The build process generates:
- **Build directory**: `cmake-build-debug/` - Contains CMake configuration and build files
- **Executable**: `cmake-build-debug/build/main.exe` - The compiled application

## Manual Building (Without Scripts)

If you prefer to build manually:

```bash
mkdir cmake-build-debug
cd cmake-build-debug
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

## Troubleshooting

- **CMake not found**: Run `pacman -S mingw-w64-x86_64-cmake` in MSYS2 terminal
- **gcc not found**: Run `pacman -S mingw-w64-x86_64-toolchain` in MSYS2 terminal
- **Curses not found**: Run `pacman -S mingw-w64-x86_64-ncurses` in MSYS2 terminal
- **Scripts not found in PATH**: Add MSYS64 MinGW bin directories to your Windows PATH environment variable

foo