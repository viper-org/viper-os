cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "main.c"
)

set(HEADERS
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(fb ${SOURCES} ${HEADERS})
set_target_properties(fb PROPERTIES OUTPUT_NAME fb.vdrv)