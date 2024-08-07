cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "source/main.cpp"

    "source/driver/debugcon.cpp"

    "source/mm/physical.cpp"
    "source/mm/heap.cpp"

    "source/fs/vfs.cpp"
    "source/fs/testfs.cpp"

    "source/libc/stdlib.cpp"
    "source/libc/string.cpp"
    "source/libc/new.cpp"
)

set(HEADERS
    "include/echis/main.h"

    "include/echis/driver/debugcon.h"

    "include/echis/mm/physical.h"
    "include/echis/mm/heap.h"

    "include/echis/fs/vfs.h"
    "include/echis/fs/testfs.h"

    "include/echis/util/math.h"
    "include/echis/util/debug.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(echis ${SOURCES} ${HEADERS})

target_include_directories(echis PRIVATE include/echis)
target_include_directories(echis PUBLIC include)
target_compile_features(echis PUBLIC cxx_std_20)

target_compile_definitions(echis PRIVATE $<$<CONFIG:Debug>:ECHIS_DEBUG_LOG_ALL>)