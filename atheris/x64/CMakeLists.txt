cmake_minimum_required(VERSION 3.26)

set(SOURCES
    "src/main.cpp"

    "src/cpu/gdt/gdt.cpp"
    "src/cpu/gdt/install_gdt.s"
    "src/cpu/interrupt/idt.cpp"
    "src/cpu/interrupt/exception.cpp"
    "src/cpu/interrupt/handlers.s"
    "src/cpu/asm.cpp"

    "src/fs/module.cpp"

    "src/mm/util.cpp"


    "src/atheris/halt.cpp"

    "src/atheris/driver/framebufferInit.cpp"
    "src/atheris/driver/consoleInit.cpp"

    "src/atheris/mm/pm.cpp"
    "src/atheris/mm/vm.cpp"

    "src/atheris/cpu/smpInit.cpp"
    "src/atheris/cpu/core.cpp"
)

set(HEADERS
    "include/cpu/gdt/gdt.h"
    "include/cpu/interrupt/idt.h"
    "include/cpu/asm.h"
    "include/cpu/context.h"

    "include/fs/module.h"

    "include/mm/util.h"


    "include/atheris/private/halt.h"

    "include/atheris/private/driver/framebufferInit.h"
    "include/atheris/private/driver/consoleInit.h"

    "include/atheris/private/mm/pm.h"
    "include/atheris/private/mm/vm.h"

    "include/atheris/private/cpu/smpInit.h"
    "include/atheris/private/cpu/core.h"


    "include/limine.h"
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
    "-ffreestanding"
    "-fno-stack-protector"
    "-fno-stack-check"
    "-fno-lto"
    "-fno-rtti"
    "-fno-exceptions"
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
target_include_directories(viperos-x64 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../public/atheris)

target_compile_options(echis PRIVATE ${COMPILER_OPTIONS})
target_include_directories(echis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../public)
target_include_directories(echis PRIVATE include)

add_custom_command(TARGET viperos-x64 POST_BUILD COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build_iso.sh)