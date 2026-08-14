#!/bin/bash
# Bash build script for FilesTUI
# Usage: ./build.sh [command]
# Commands: build (default), clean, rebuild, run

COMMAND="${1:-build}"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build"
BINARY_PATH="${BUILD_DIR}/build/main.exe"

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

build() {
    echo -e "${GREEN}Building FilesTUI...${NC}"
    
    if [ ! -d "$BUILD_DIR" ]; then
        echo -e "${YELLOW}Creating build directory...${NC}"
        mkdir -p "$BUILD_DIR"
    fi
    
    cd "$BUILD_DIR"
    
    echo -e "${YELLOW}Running CMake configuration...${NC}"
    cmake -G "MinGW Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER="C:/msys64/mingw64/bin/gcc.exe" \
        -DCMAKE_CXX_COMPILER="C:/msys64/mingw64/bin/g++.exe" \
        -DCMAKE_MAKE_PROGRAM="C:/msys64/mingw64/bin/mingw32-make.exe" ..
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}CMake configuration failed!${NC}"
        cd "$PROJECT_ROOT"
        exit 1
    fi
    
    echo -e "${YELLOW}Building project...${NC}"
    cmake --build .
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}Build completed successfully!${NC}"
        echo -e "${CYAN}Binary location: $BINARY_PATH${NC}"
    else
        echo -e "${RED}Build failed!${NC}"
        cd "$PROJECT_ROOT"
        exit 1
    fi
    
    cd "$PROJECT_ROOT"
}

clean() {
    echo -e "${YELLOW}Cleaning build files...${NC}"
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        echo -e "${GREEN}Clean completed!${NC}"
    else
        echo -e "${YELLOW}No build directory found.${NC}"
    fi
}

rebuild() {
    clean
    build
}

run() {
    if [ ! -f "$BINARY_PATH" ]; then
        echo -e "${YELLOW}Binary not found. Building first...${NC}"
        build
    fi
    
    echo -e "${GREEN}Running application...${NC}"
    "$BINARY_PATH"
}

case "$COMMAND" in
    build)
        build
        ;;
    clean)
        clean
        ;;
    rebuild)
        rebuild
        ;;
    run)
        run
        ;;
    *)
        echo -e "${RED}Unknown command: $COMMAND${NC}"
        echo -e "${YELLOW}Available commands: build, clean, rebuild, run${NC}"
        exit 1
        ;;
esac
