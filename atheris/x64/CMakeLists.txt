cmake_minimum_required(VERSION 3.26)

set(SOURCES
    "src/main.cpp"
)

set(HEADERS
    "include/limine.h"
)

set(LINKER_OPTIONS
    "-Wl,-melf_x86_64"
    "-nostdlib"
    "-static"
    "-Wl,--no-dynamic-linker"
    "-ztext"
    "-zmax-page-size=0x1000"
    "-Tlinker.ld"
)

set(COMPILER_OPTIONS
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-ffreestanding"
    "-fno-stack-protector"
    "-fno-stack-check"
    "-fno-lto"
    "-fno-rtti"
    "-fno-exceptions"
    "-m64"
    "-march=x86-64"
    "-mno-80387"
    "-mno-mmx"
    "-mno-sse"
    "-mno-sse2"
    "-mno-red-zone"
)

set(CMAKE_CXX_COMPILER x86_64-elf-g++)
    
execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_limine_header.sh)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(viperos-x64 ${SOURCES} ${HEADERS})
target_include_directories(viperos-x64
    PUBLIC
        include
)

target_link_libraries(viperos-x64 PRIVATE echis)
target_link_options(viperos-x64 PRIVATE ${LINKER_OPTIONS})
target_compile_options(viperos-x64 PRIVATE ${COMPILER_OPTIONS})

target_compile_options(echis PRIVATE ${COMPILER_OPTIONS})

add_custom_command(TARGET viperos-x64 POST_BUILD COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build_iso.sh)