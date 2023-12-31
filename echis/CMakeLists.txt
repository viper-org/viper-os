cmake_minimum_required(VERSION 3.26)

set(SOURCES
    "src/core/kernel.cpp"

    "src/core/exception/exception.cpp"

    "src/mm/pmm.cpp"
    "src/mm/heap.cpp"

    "src/driver/framebuffer.cpp"
    "src/driver/console.cpp"

    "src/driver/impl/framebuffer.cpp"
    "src/driver/impl/console.cpp"


    "src/std/thread/mutex.cpp"

    
    "src/libc/string.cpp"
    "src/libc/stdio.cpp"
)

set(HEADERS
    "include/echis/core/kernel.h"

    "include/echis/core/exception/exception.h"

    "include/echis/mm/pmm.h"
    "include/echis/mm/physaddr.h"
    "include/echis/mm/heap.h"

    "include/echis/driver/framebuffer.h"
    "include/echis/driver/console.h"

    "include/echis/driver/impl/framebuffer.h"
    "include/echis/driver/impl/console.h"


    "include/echis/std/container/atomic.h"
    "include/echis/std/thread/mutex.h"


    "include/libc/string.h"
    "include/libc/stdio.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(echis ${SOURCES} ${HEADERS})
target_include_directories(echis
    PUBLIC
        include
)
target_include_directories(echis PUBLIC include/libc)

target_include_directories(echis
    PUBLIC
        include/echis
)

target_compile_features(echis PUBLIC cxx_std_20)