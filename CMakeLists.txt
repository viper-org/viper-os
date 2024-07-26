cmake_minimum_required(VERSION 3.8)

project(viper-os C CXX ASM)

add_subdirectory(echis)

add_subdirectory(atheris/${arch})