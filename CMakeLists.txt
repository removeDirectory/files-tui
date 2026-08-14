cmake_minimum_required(VERSION 3.20)

project(
  FilesTUI
  VERSION 0.0.1
  DESCRIPTION "TUI-Based File Explorer"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

add_subdirectory(src)

add_executable(main main.c)
target_link_libraries(main PRIVATE src)
# Link ncurses DLL import library
target_link_libraries(main PRIVATE "C:/msys64/mingw64/lib/libncursesw.dll.a")
target_include_directories(main PRIVATE "C:/msys64/mingw64/include/ncurses")
