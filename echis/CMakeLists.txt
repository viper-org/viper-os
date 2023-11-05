cmake_minimum_required(VERSION 3.26)

set(SOURCES
    "src/mm/pmm.cpp"

    "src/driver/framebuffer.cpp"
    "src/driver/console.cpp"

    
    "src/libc/string.cpp"
)

set(HEADERS
    "include/echis/mm/pmm.h"

    "include/echis/driver/framebuffer.h"
    "include/echis/driver/console.h"


    "include/string.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(echis ${SOURCES} ${HEADERS})
target_include_directories(echis
    PUBLIC
        include
)

target_include_directories(echis
    PRIVATE
        include/echis
)

target_compile_features(echis PUBLIC cxx_std_20)