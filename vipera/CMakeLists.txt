cmake_minimum_required(VERSION 3.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

add_subdirectory(${arch})

include_directories(common/include)
# These are set by the arch
add_link_options(${LINKER_OPTIONS})
add_compile_options(${COMPILER_OPTIONS})

add_subdirectory(common)