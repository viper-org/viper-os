all:
	mkdir -p build
	make -C tty
	make -C libc
	gcc -isystem libc/include libc/src/crt0.o -nostdlib -nostdinc -fno-stack-protector -pie -Wl,--no-dynamic-linker -ggdb3 test.c -o build/test -Llib -lc