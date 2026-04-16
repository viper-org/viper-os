cmake_minimum_required(VERSION 3.8)

project(viperos C ASM)

add_subdirectory(drivers)
add_subdirectory(user)
add_subdirectory(krnl)

add_custom_target(viperos
    DEPENDS all
)