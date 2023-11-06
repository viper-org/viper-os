cmake_minimum_required(VERSION 3.26)

project(viper-os C CXX ASM)

add_subdirectory(echis)

if (${arch} MATCHES "tests")
    add_subdirectory(tests)
else()
    add_subdirectory(atheris/${arch})
endif()