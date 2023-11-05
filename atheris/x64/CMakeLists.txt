cmake_minimum_required(VERSION 3.26)

set(SOURCES
    "src/main.cpp"
    
    "src/driver/consoleInstall.cpp"
    "src/driver/framebufferInstall.cpp"

    "src/fs/module.cpp"


    "src/atheris/halt.cpp"
)

set(HEADERS
    "include/driver/framebufferInstall.h"
    "include/driver/consoleInstall.h"

    "include/fs/module.h"


    "include/atheris/halt.h"


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
target_include_directories(viperos-x64 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../public)

target_compile_options(echis PRIVATE ${COMPILER_OPTIONS})
target_include_directories(echis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../public)

add_custom_command(TARGET viperos-x64 POST_BUILD COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build_iso.sh)