cmake_minimum_required(VERSION 3.8)

project(viper-os C ASM)

set(SOURCES
    "source/init.c"

    "source/cpu/install_gdt.s"
    "source/cpu/gdt.c"
    "source/cpu/interrupt/idt.c"
    "source/cpu/interrupt/exception.c"
    "source/cpu/interrupt/interrupt_table.s"

    "source/driver/dbg.c"

    "source/mm/pm.c"
    "source/mm/vm.c"
    "source/mm/kheap.c"

    "source/libc/string.c"
)

set(HEADERS
    "include/cpu/gdt.h"
    "include/cpu/idt.h"
    "include/cpu/cpu.h"

    "include/driver/dbg.h"
    "include/driver/port.h"

    "include/mm/pm.h"
    "include/mm/vm.h"
    "include/mm/kheap.h"

    "include/stdint.h"
    "include/stdarg.h"
    "include/stdbool.h"
    "include/string.h"
)   

set(LINKER_OPTIONS
    "-Wl,-melf_x86_64"
    "-nostdlib"
    "-static"
    "-no-pie"
    "-Wl,--no-dynamic-linker"
    "-ztext"
    "-zmax-page-size=0x1000"
    "-Tlinker.ld"
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
    "-fno-pie"
    "-fno-pic"
    "-m64"
    "-march=x86-64"
    "-mno-80387"
    "-mno-mmx"
    "-mno-sse"
    "-mno-sse2"
    "-mno-red-zone"
    "-mcmodel=kernel"
)

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_limine_header.sh ${CMAKE_CURRENT_SOURCE_DIR})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(viperos-amd64 ${SOURCES} ${HEADERS})
target_include_directories(viperos-amd64 PUBLIC include)

target_link_options(viperos-amd64 PRIVATE ${LINKER_OPTIONS})
target_compile_options(viperos-amd64 PRIVATE ${COMPILER_OPTIONS})

add_custom_command(TARGET viperos-amd64 POST_BUILD COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build_iso.sh)