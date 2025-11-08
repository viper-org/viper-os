# ViperOS


## About
ViperOS is a multitasking operating system that currently only supports the amd64 architecture, however more architectures will be supported in future.

## Building

ViperOS requires any C compiler that can target amd64, as well as the following dependencies:
* CMake
* QEMU

To build, simply run `cmake .`, then `cmake --build .`.

If compilation is slow, use the `-j` flag when running `cmake --build`

## Running

There are shell scripts to run ViperOS in QEMU in both debug and release mode: `./krnl/debug.sh` and `./krnl/run.sh` respectively.

When running in debug mode, the emulator is paused on open, so connect to it using gdb(`target remote localhost:1234`) and then use `continue` in GDB to begin execution.