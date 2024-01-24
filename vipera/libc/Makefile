CC=gcc
AS=as
AR=ar

CFLAGS=-nostdlib -nostdinc -g -fno-stack-protector -pie -Iinclude
ASFLAGS=
ARFLAGS=rcs

CFILES:=$(shell find src -name '*.c')
ASFILES:=$(shell find src -name '*.s')
OBJS:=$(CFILES:.c=.o) $(ASFILES:.s=.o)
HEADERS:=$(shell echo ${CFILES:.c=.d})

TARGET=../lib/libc.a

.PHONY: all

all: $(TARGET)

-include $(HEADERS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^