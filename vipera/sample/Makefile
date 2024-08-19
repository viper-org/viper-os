CFLAGS=-Wall \
    -Wextra \
    -Wpedantic \
    -O0 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-stack-check \
    -fno-lto \
    -fno-pie \
    -fno-pic \
    -m64 \
    -march=x86-64 \
    -mno-80387 \
    -mno-mmx \
    -mno-sse \
    -mno-sse2 \
    -mno-red-zone \
    -mcmodel=kernel \

LDFLAGS=-Wl,-melf_x86_64 \
    -nostdlib \
    -static \
    -no-pie \
    -Wl,--no-dynamic-linker \
    -ztext \
    -zmax-page-size=0x1000 \
    -Tlinker.ld \

all: main.c ../driver.h
	x86_64-elf-gcc $(CFLAGS) -g -c main.c -I.. -o main.o
	x86_64-elf-gcc $(LDFLAGS) main.o -o ../sample.elf