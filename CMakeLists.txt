cmake_minimum_required(VERSION 3.30)

project(
  FilesTUI
  VERSION 0.0.1
  DESCRIPTION "TUI-Based File Explorer"
)

add_subdirectory(src)

add_executable(main main.c)
target_link_libraries(main PRIVATE src)