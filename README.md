# Viper OS

## About
ViperOS is an operating system that currently runs on amd64 exclusively, however other architectures will be implemented in the future.
It runs with a custom kernel(echis) that is architecture-independent.

ViperOS has its own userspace(vipera) that aims to be reliable and compatible with other operating systems.


## Features
- Multithreaded kernel
- Custom userspace(including a terminal emulator)


## TODO
- [ ] Process spawning
- [ ] Shell
- [ ] Process groups
- [ ] User IDs
- [ ] Disk Driver(NVMe)
- [ ] FAT32 Driver


## Building

To build ViperOS, you will need the following dependencies:

- CMake

To configure the CMakeLists for a specific architecture, run the script `cmake/run_cmake_$(arch).sh`

Then, simply run `cmake --build .` which will build the kernel.


## Running

To run ViperOS, you will require an emulator such as qemu(it is untested on real hardware).

There are shell scripts in each of the architecture directories to run ViperOS in an emulator. Run `atheris/$(arch)/run.sh` to run.

Alternatively, to debug, run the script `atheris/$(arch)/debug.sh`.
