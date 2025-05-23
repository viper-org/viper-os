cmake_minimum_required(VERSION 3.8)

set(SOURCES
    "source/boot/init.cpp"

    "source/atheris/driver/debugcon.cpp"
)

set(HEADERS
    "include/atheris/private/driver/debugcon.h"

    "include/driver/port.h"
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
    "-nostdinc++"
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

add_custom_command(TARGET viperos-amd64 POST_BUILD COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build_iso.sh)