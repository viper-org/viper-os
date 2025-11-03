cmake_minimum_required(VERSION 3.8)

set(LINKER_OPTIONS
    "-Wl,-melf_x86_64"
    "-nostdlib"
    "-ztext"
    "-pie"
    "-zmax-page-size=0x1000"
    "--sysroot=../sysroot"
    "-Wl,--dynamic-linker=/lib/ld.so"
)

set(COMPILER_OPTIONS
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-O0"
    "-nostdinc"
    "-fno-stack-protector"
    "-fno-stack-check"
    "-fno-lto"
    "-fPIC"
    "-fPIE"
    "-m64"
    "-march=x86-64"
    "-mno-80387"
    "-mno-mmx"
    "-mno-sse"
    "-mno-sse2"
    "-mno-red-zone"
    "--sysroot=../sysroot"
)

set(SOURCES
    "source/tty.c"
    "source/screen.c"
    "source/font.c"
)

set(HEADERS
    "include/font.h"
    "include/screen.h"
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../sysroot/bin)

add_executable(tty ${SOURCES} ${HEADERS})
target_link_libraries(tty PRIVATE c)
target_include_directories(tty PRIVATE include)

target_link_options(tty PRIVATE ${LINKER_OPTIONS})
target_compile_options(tty PRIVATE ${COMPILER_OPTIONS})