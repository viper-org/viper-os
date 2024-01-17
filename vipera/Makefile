all: test.c Makefile
	mkdir -p build
	gcc -nostdlib -nostdinc -pie test.c -fno-stack-protector -o build/test.exe