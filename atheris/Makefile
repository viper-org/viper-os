override VPR=../viper-lang-build/compiler/viper
override LD=ld

override VPRFLAGS += \
	-Iinclude

override LDFLAGS += \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	--no-dynamic-linker \
	-z text \
	-z max-page-size=0x1000 \
	-T linker.ld

override KERNEL=atheris.elf

override VPRFILES:=$(shell find -L src -type f -name '*.vpr')
override OBJ:=$(VPRFILES:.vpr=.o)

.PHONY: all
all: $(KERNEL)

$(KERNEL): Makefile linker.ld $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

%.o: %.vpr Makefile
	$(VPR) $(VPRFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ)

.PHONY: disclean
distclean: clean