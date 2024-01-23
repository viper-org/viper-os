all:
	mkdir -p build
	make -C tty
	gcc -nostdlib -nostdinc -fno-stack-protector -pie -Wl,--no-dynamic-linker -ggdb3 test.c -o build/test