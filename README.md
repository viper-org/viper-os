# ViperOS


## About
ViperOS is a multitasking operating system that currently only supports the amd64 architecture, however more architectures will be supported in future.

## Building

ViperOS requires a C++ compiler that supports at least C++20, along with the other following dependencies:
* CMake
* QEMU

To build for a specific architecture, simply run `./cmake/run_cmake_{arch}.sh`, where arch is the name of the architecture, e.g. `amd64`

Then run the `cmake --build .` command to build

## Running

Each architecture has a shell script to run the ViperOS in QEMU after the build command has been run - `./atheris/{arch}/run.sh`