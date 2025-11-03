cmake_minimum_required(VERSION 3.8)

set(LINKER_OPTIONS
    "-Wl,-melf_x86_64"
    "-nostdlib"
    "-shared"
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
    "-m64"
    "-march=x86-64"
    "-mno-80387"
    "-mno-mmx"
    "-mno-sse"
    "-mno-sse2"
    "-mno-red-zone"
)

set(SOURCES
    "source/sys/mman.c"
    "source/sys/ioctl.c"
    "source/sys/syscall.s"

    "source/unistd/file.c"
    "source/unistd/proc.c"

    "source/poll.c"
    "source/sched.c"
    "source/string.c"
)

set(HEADERS
    "include/sys/mman.h"
    "include/sys/ioctl.h"
    "include/sys/syscall.h"

    "include/poll.h"
    "include/sched.h"
    "include/unistd.h"
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../sysroot/lib)

add_library(c SHARED ${SOURCES} ${HEADERS})

target_include_directories(c PUBLIC include)
target_link_options(c PRIVATE ${LINKER_OPTIONS})
target_compile_options(c PRIVATE ${COMPILER_OPTIONS})