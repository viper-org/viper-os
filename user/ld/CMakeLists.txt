cmake_minimum_required(VERSION 3.8)

set(LINKER_OPTIONS
    "-Wl,-melf_x86_64"
    "-Wl,--no-dynamic-linker"
    "-nostdlib"
    "-shared"
    "-pie"
    "-ztext"
    "-zmax-page-size=0x1000"
)

set(COMPILER_OPTIONS
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-O0"
    "-nostdinc"
    "-ffreestanding"
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
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../sysroot/lib)

add_executable(ld main.c start.s)
set_target_properties(ld PROPERTIES OUTPUT_NAME "ld.so")

target_link_options(ld PRIVATE ${LINKER_OPTIONS})
target_compile_options(ld PRIVATE ${COMPILER_OPTIONS})