CC=gcc
LD=ld

CFLAGS=-nostdlib -nostdinc -fno-stack-protector -pie -Iinclude
LDFLAGS=-nostdlib -pie

CFILES:=$(shell find src -name '*.c')
OBJS:=$(CFILES:.c=.o)
HEADERS:=$(shell echo ${CFILES:.c=.d})

TARGET=../build/tty

.PHONY: all

all: $(TARGET)

-include $(HEADERS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@