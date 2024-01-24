all:
	mkdir -p build
	make -C tty
	make -C libc
	gcc -isystem libc/include -nostdlib -nostdinc -fno-stack-protector -pie -Wl,--no-dynamic-linker -ggdb3 test.c -o build/test -Llib -lc