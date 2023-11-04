cmake_minimum_required(VERSION 3.26)

set(SOURCES
    "src/mm/pmm.cpp"
)

set(HEADERS
    "include/mm/pmm.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(echis ${SOURCES} ${HEADERS})
target_include_directories(echis
    PUBLIC
        include
)

target_compile_features(echis PUBLIC cxx_std_20)