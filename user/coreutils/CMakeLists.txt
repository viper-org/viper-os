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

set(PROGRAMS
    "ls"
    "echo"
    "hello"
    "mkdir"
    "pwd"
    "touch"
    "cat"
)

foreach (PROG IN LISTS PROGRAMS)
    add_executable(${PROG} ${PROG}.c)
    target_link_libraries(${PROG} PRIVATE c)
    target_link_options(${PROG} PRIVATE ${LINKER_OPTIONS})
    target_compile_options(${PROG} PRIVATE ${COMPILER_OPTIONS})
endforeach()