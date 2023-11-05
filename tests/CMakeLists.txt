cmake_minimum_required(VERSION 3.26)

Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

set(SOURCES
    "src/mm/pmmTests.cpp"
    
    "src/driver/framebufferTests.cpp"
    "src/driver/consoleTests.cpp"


    "src/mock/echis/driver/framebuffer.cpp"
    "src/mock/echis/driver/console.cpp"
)

set(HEADERS
    "include/mock/echis/driver/framebuffer.h"
    "include/mock/echis/driver/console.h"
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(viperos-tests ${SOURCES} ${HEADERS})
target_include_directories(viperos-tests
    PUBLIC
        include
)

target_link_libraries(viperos-tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(viperos-tests PRIVATE echis)
target_link_libraries(viperos-tests PRIVATE gcov)
target_compile_definitions(viperos-tests PRIVATE VIPEROS_ENABLE_TEST_FUNCTIONS)
target_compile_options(viperos-tests PRIVATE -fprofile-arcs -ftest-coverage)

target_compile_definitions(echis PRIVATE VIPEROS_ENABLE_TEST_FUNCTIONS)
target_compile_options(echis PRIVATE -fprofile-arcs -ftest-coverage)