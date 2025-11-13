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
    "-nostdlib"
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
    "source/sys/wait.c"
    "source/sys/stat.c"
    "source/sys/syscall.s"

    "source/unistd/file.c"
    "source/unistd/proc.c"

    "source/poll.c"
    "source/sched.c"
    "source/dirent.c"
    "source/readline.c"

    "source/string.c"
    "source/stdio.c"
    "source/stdlib.c"
    "source/signal.c"
    "source/ctype.c"
)

set(HEADERS
    "include/sys/mman.h"
    "include/sys/ioctl.h"
    "include/sys/wait.h"
    "include/sys/stat.h"
    "include/sys/syscall.h"

    "include/poll.h"
    "include/sched.h"
    "include/unistd.h"
    "include/dirent.h"
    "include/readline.h"

    "include/string.h"
    "include/stdio.h"
    "include/stdlib.h"
    "include/signal.h"
    "include/stdint.h"
    "include/stddef.h"
    "include/ctype.h"
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../sysroot/lib)

add_library(crt0 OBJECT source/crt0.s)
add_library(c SHARED ${SOURCES} ${HEADERS})
target_sources(c INTERFACE $<TARGET_OBJECTS:crt0>)

target_include_directories(c PUBLIC include)
target_link_options(c PRIVATE ${LINKER_OPTIONS})
target_compile_options(c PRIVATE ${COMPILER_OPTIONS})