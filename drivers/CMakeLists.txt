cmake_minimum_required(VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

add_link_options(
    "-Wl,-melf_x86_64"
    "-nostdlib"
    "-pie"
    "-Wl,--no-dynamic-linker"
    "-ztext"
    "-zmax-page-size=0x1000"
    "-T${CMAKE_CURRENT_LIST_DIR}/linker.ld"
)

add_compile_options(
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    "-O0"
    "-nostdinc"
    "-ffreestanding"
    "-fno-stack-protector"
    "-fno-stack-check"
    "-fno-lto"
    "-fPIE"
    "-fPIC"
    "-m64"
    "-march=x86-64"
    "-mno-80387"
    "-mno-mmx"
    "-mno-sse"
    "-mno-sse2"
    "-mno-red-zone"
)

include_directories(include)

FILE(GLOB drivers ABSOLUTE ${CMAKE_CURRENT_LIST_DIR}/drivers/ ${CMAKE_CURRENT_LIST_DIR}/drivers/*)

foreach(D ${drivers})
    if (IS_DIRECTORY ${D})
        add_subdirectory(${D})
    endif()
endforeach()