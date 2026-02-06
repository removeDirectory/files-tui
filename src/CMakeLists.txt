set(HEADER_LIST "../include/src.h")

add_library(src src.c ${HEADER_LIST})
target_include_directories(src PUBLIC ../include)

# NOTE: add target_link_libraries() when needed here