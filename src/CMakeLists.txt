set(HEADER_LIST "../include/src.h")

add_library(src src.c ${HEADER_LIST})
# Link ncurses DLL import library
target_link_libraries(src PUBLIC "C:/msys64/mingw64/lib/libncursesw.dll.a")
target_include_directories(src PUBLIC "../include" "C:/msys64/mingw64/include/ncurses")

# NOTE: add target_link_libraries() when needed here
