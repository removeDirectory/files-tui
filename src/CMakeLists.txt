set(HEADER_LIST "../include/src.h")

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_library(src src.c ${HEADER_LIST})
target_link_libraries(src ${CURSES_LIBRARIES})
target_include_directories(src PUBLIC ../include)

# NOTE: add target_link_libraries() when needed here
