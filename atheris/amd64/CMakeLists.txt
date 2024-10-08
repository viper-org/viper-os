cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "source/boot/start.cpp"

    "source/driver/acpi.cpp"
    "source/driver/hpet.cpp"

    "source/atheris/driver/debugcon.cpp"
    "source/atheris/driver/timer.cpp"

    "source/atheris/mm/vm.cpp"

    "source/atheris/sched/sched.cpp"
    "source/atheris/sched/prepare_thread.s"
    "source/atheris/sched/switch_context.s"

    "source/atheris/cpu/halt.cpp"
    "source/atheris/cpu/int.cpp"

    "source/cpu/gdt.cpp"
    "source/cpu/gdt_install.s"
    "source/cpu/asm.cpp"
    "source/cpu/interrupt/idt.cpp"
    "source/cpu/interrupt/interrupt_table.s"
    "source/cpu/interrupt/exception.cpp"
    "source/cpu/interrupt/irq.cpp"
    "source/cpu/interrupt/apic.cpp"

    "source/mm/pm.cpp"
)

set(HEADERS
    "include/limine.h"

    "include/driver/port.h"
    "include/driver/acpi.h"
    "include/driver/hpet.h"

    "include/cpu/gdt.h"
    "include/cpu/asm.h"
    "include/cpu/interrupt/idt.h"
    "include/cpu/interrupt/apic.h"

    "include/mm/pm.h"


    "include/atheris/private/driver/debugcon.h"
    "include/atheris/private/driver/timer.h"

    "include/atheris/private/mm/pm.h"
    "include/atheris/private/mm/vm.h"

    "include/atheris/private/sched/sched.h"

    "include/atheris/private/cpu/halt.h"
    "include/atheris/private/cpu/int.h"
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

execute_process(COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/get_limine_header.sh ${CMAKE_CURRENT_SOURCE_DIR})

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(viperos-amd64 ${SOURCES} ${HEADERS})
target_include_directories(viperos-amd64 PUBLIC include)

target_link_libraries(viperos-amd64 PRIVATE echis)
target_link_options(viperos-amd64 PRIVATE ${LINKER_OPTIONS})
target_compile_options(viperos-amd64 PRIVATE ${COMPILER_OPTIONS})
target_include_directories(viperos-amd64 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../public)

#target_compile_definitions(viperos-amd64 PRIVATE $<$<CONFIG:Debug>:ATHERIS_DEBUG_LOG_ALL>)

target_compile_options(echis PRIVATE ${COMPILER_OPTIONS})
target_include_directories(echis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../public)
target_include_directories(echis PRIVATE include)

# TODO: Run this on updates to vipera so drivers can be reloaded
add_custom_command(TARGET viperos-amd64 POST_BUILD COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build_iso.sh)