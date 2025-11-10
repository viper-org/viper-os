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
    "-nostdlib"
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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../sysroot/bin)

add_executable(sh sh.c)
target_link_libraries(sh PRIVATE c)
target_link_options(sh PRIVATE ${LINKER_OPTIONS})
target_compile_options(sh PRIVATE ${COMPILER_OPTIONS})

add_executable(hello hello.c)
target_link_libraries(hello PRIVATE c)
target_link_options(hello PRIVATE ${LINKER_OPTIONS})
target_compile_options(hello PRIVATE ${COMPILER_OPTIONS})