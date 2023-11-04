cmake_minimum_required(VERSION 3.26)

Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

set(SOURCES
    "src/test.cpp"
)

set(HEADERS
)

source_group(TREE ${PROJECT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(viperos-tests ${SOURCES} ${HEADERS})
target_include_directories(viperos-tests
    PUBLIC
        include
)

target_link_libraries(viperos-tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(viperos-tests PRIVATE echis)