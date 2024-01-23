cmake_minimum_required(VERSION 3.26)

set(SOURCES
    "src/core/kernel.cpp"
    "src/core/exception/exception.cpp"

    "src/mm/pmm.cpp"
    "src/mm/heap.cpp"
    "src/mm/vmm.cpp"

    "src/sched/process.cpp"
    "src/sched/pipe.cpp"
    "src/sched/sched.cpp"

    "src/fs/vfs.cpp"
    "src/fs/tmpfs.cpp"
    "src/fs/devfs.cpp"
    "src/fs/initrd.cpp"

    "src/ldr/elf.cpp"

    "src/event/write.cpp"

    "src/signal/signal.cpp"

    "src/driver/framebuffer.cpp"
    "src/driver/console.cpp"

    "src/driver/impl/framebuffer.cpp"
    "src/driver/impl/console.cpp"


    "src/syscall/file/read.cpp"
    "src/syscall/file/write.cpp"
    "src/syscall/file/open.cpp"
    "src/syscall/file/close.cpp"
    "src/syscall/file/ioctl.cpp"
    "src/syscall/file/seek.cpp"
    "src/syscall/file/poll.cpp"

    "src/syscall/mem/mmap.cpp"

    "src/syscall/proc/pipe.cpp"
    "src/syscall/proc/spawn.cpp"
    "src/syscall/proc/sigaction.cpp"
    "src/syscall/proc/raise.cpp"
    "src/syscall/proc/exit.cpp"


    "src/std/container/string.cpp"

    "src/std/thread/mutex.cpp"

    
    "src/libc/string.cpp"
    "src/libc/stdio.cpp"
)

set(HEADERS
    "include/echis/core/kernel.h"

    "include/echis/core/exception/exception.h"

    "include/echis/mm/pmm.h"
    "include/echis/mm/physaddr.h"
    "include/echis/mm/heap.h"
    "include/echis/mm/vmm.h"

    "include/echis/sched/process.h"
    "include/echis/sched/processFd.h"
    "include/echis/sched/pipe.h"
    "include/echis/sched/sched.h"

    "include/echis/fs/vfs.h"
    "include/echis/fs/tmpfs.h"
    "include/echis/fs/devfs.h"
    "include/echis/fs/initrd.h"

    "include/echis/ldr/elf.h"

    "include/echis/event/write.h"

    "include/echis/signal/signal.h"

    "include/echis/driver/framebuffer.h"
    "include/echis/driver/console.h"

    "include/echis/driver/impl/framebuffer.h"
    "include/echis/driver/impl/console.h"


    "include/echis/syscall/file/read.h"
    "include/echis/syscall/file/write.h"
    "include/echis/syscall/file/open.h"
    "include/echis/syscall/file/close.h"
    "include/echis/syscall/file/ioctl.h"
    "include/echis/syscall/file/seek.h"
    "include/echis/syscall/file/poll.h"

    "include/echis/syscall/mem/mmap.h"

    "include/echis/syscall/proc/pipe.h"
    "include/echis/syscall/proc/spawn.h"
    "include/echis/syscall/proc/sigaction.h"
    "include/echis/syscall/proc/raise.h"
    "include/echis/syscall/proc/exit.h"


    "include/echis/std/container/atomic.h"
    "include/echis/std/container/ring.h"
    "include/echis/std/container/string.h"
    "include/echis/std/container/vector.h"
    "include/echis/std/container/ptr.h"
    "include/echis/std/thread/mutex.h"


    "include/libc/string.h"
    "include/libc/stdio.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(echis ${SOURCES} ${HEADERS})
target_include_directories(echis
    PUBLIC
        include
)
target_include_directories(echis PUBLIC include/libc)

target_include_directories(echis
    PUBLIC
        include/echis
)

target_compile_features(echis PUBLIC cxx_std_20)