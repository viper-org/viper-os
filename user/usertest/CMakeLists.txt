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
    "--sysroot=../sysroot "
)
set(ENV{LD_BIND_NOW} 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../sysroot/bin)

add_executable(usertest test.c)
target_link_libraries(usertest PRIVATE c)

target_link_options(usertest PRIVATE ${LINKER_OPTIONS})
target_compile_options(usertest PRIVATE ${COMPILER_OPTIONS})