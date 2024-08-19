cmake_minimum_required(VERSION 3.8)

set(LINKER_OPTIONS
    "-Wl,-melf_x86_64"
    "-nostdlib"
    "-static"
    "-no-pie"
    "-Wl,--no-dynamic-linker"
    "-ztext"
    "-zmax-page-size=0x1000"
    "-Tlinker.ld"
PARENT_SCOPE)

set(COMPILER_OPTIONS
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-O0"
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
PARENT_SCOPE)

include_directories(../common/include)
add_link_options(${LINKER_OPTIONS})
add_compile_options(${COMPILER_OPTIONS})

set(DRIVERS
)

foreach(D IN LISTS DRIVERS)
    add_subdirectory(drivers/${D})
endforeach()