cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "source/init.c"

    "source/cpu/install_gdt.s"
    "source/cpu/gdt.c"
    "source/cpu/tss.c"
    "source/cpu/interrupt/idt.c"
    "source/cpu/interrupt/lapic.c"
    "source/cpu/interrupt/ioapic.c"
    "source/cpu/interrupt/exception.c"
    "source/cpu/interrupt/irq.c"
    "source/cpu/cpu.c"
    "source/cpu/syscall.s"
    "source/cpu/interrupt/interrupt_table.s"

    "source/acpi/acpi.c"
    "source/acpi/hpet.c"
    "source/acpi/madt.c"

    "source/syscall/syscall.c"
    "source/syscall/file/open.c"
    "source/syscall/file/read.c"
    "source/syscall/file/write.c"
    "source/syscall/file/close.c"
    "source/syscall/file/stat.c"
    "source/syscall/file/lseek.c"
    "source/syscall/file/poll.c"
    "source/syscall/file/pipe.c"
    "source/syscall/file/ioctl.c"
    "source/syscall/file/dup.c"
    "source/syscall/file/mkdir.c"
    "source/syscall/mem/mmap.c"
    "source/syscall/mem/munmap.c"
    "source/syscall/proc/getcwd.c"
    "source/syscall/proc/chdir.c"
    "source/syscall/sched/yield.c"
    "source/syscall/sched/exit.c"
    "source/syscall/sched/wait.c"
    "source/syscall/sched/getpid.c"
    "source/syscall/sched/spawn.c"

    "source/driver/dbg.c"
    "source/driver/keyboard.c"
    "source/driver/ldr/loader.c"
    "source/driver/helpers/fb.c"

    "source/mm/pm.c"
    "source/mm/vm.c"
    "source/mm/kheap.c"

    "source/fs/vfs.c"
    "source/fs/testfs.c"
    "source/fs/devfs.c"
    "source/fs/module.c"
    "source/fs/initrd.c"

    "source/sched/proc.c"
    "source/sched/sched.c"
    "source/sched/pipe.c"
    "source/sched/prepare_thread.s"
    "source/sched/swtch.s"
    "source/sched/idle.s"
    "source/sched/user.s"

    "source/thread/proc.c"

    "source/ldr/elf.c"

    "source/event/object.c"
    "source/event/bus.c"

    "source/libc/string.c"
)

set(HEADERS
    "include/cpu/gdt.h"
    "include/cpu/tss.h"
    "include/cpu/idt.h"
    "include/cpu/int/lapic.h"
    "include/cpu/int/ioapic.h"
    "include/cpu/cpu.h"
    "include/cpu/asm.h"

    "include/driver/dbg.h"
    "include/driver/port.h"
    "include/driver/keyboard.h"
    "include/driver/ldr/loader.h"

    "include/acpi/acpi.h"
    "include/acpi/hpet.h"
    "include/acpi/madt.h"

    "include/syscall/syscalls.h"

    "include/mm/pm.h"
    "include/mm/vm.h"
    "include/mm/kheap.h"

    "include/fs/vfs.h"
    "include/fs/testfs.h"
    "include/fs/devfs.h"
    "include/fs/module.h"
    "include/fs/initrd.h"

    "include/sched/proc.h"
    "include/sched/procfd.h"
    "include/sched/sched.h"
    "include/sched/pipe.h"
    
    "include/thread/proc.h"

    "include/ldr/elf.h"

    "include/event/object.h"
    "include/event/bus.h"

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