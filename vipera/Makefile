all:
	mkdir -p build
	make -C tty
	gcc -nostdlib -nostdinc -fno-stack-protector -pie test.c -o build/test