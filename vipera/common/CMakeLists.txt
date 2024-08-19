cmake_minimum_required(VERSION 3.8)

FILE(GLOB drivers ABSOLUTE ${CMAKE_CURRENT_LIST_DIR}/drivers/ ${CMAKE_CURRENT_LIST_DIR}/drivers/*)

foreach(D ${drivers})
    if (IS_DIRECTORY ${D})
        add_subdirectory(${D})
    endif()
endforeach()