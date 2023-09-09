override MAKEFLAGS += -rR

override KERNEL := kernel.elf

define DEFAULT_VAR =
	ifeq ($(origin $1),default)
		override $(1) := $(2)
	endif
	ifeq ($(origin $1),undefined)
		override $(1) := $(2)
	endif
endef

override DEFAULT_CXX := c++
$(eval $(call DEFAULT_VAR,CXX,$(DEFAULT_CXX)))

override DEFAULT_LD := ld
$(eval $(call DEFAULT_VAR,LD,$(DEFAULT_LD)))

override DEFAULT_CFLAGS := -g -O2 -pipe
$(eval $(call DEFAULT_VAR,CFLAGS,$(DEFAULT_CFLAGS)))

override DEFAULT_CPPFLAGS :=
$(eval $(call DEFAULT_VAR,CPPFLAGS,$(DEFAULT_CPPFLAGS)))

override DEFAULT_NASMFLAGS := -F dwarf -g
$(eval $(call DEFAULT_VAR,NASMFLAGS,$(DEFAULT_NASMFLAGS)))

override DEFAULT_LDFLAGS :=
$(eval $(call DEFAULT_VAR,LDFLAGS,$(DEFAULT_LDFLAGS)))

override CFLAGS += \
	-Wall \
	-Wextra \
	-std=c++20 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-exceptions \
	-fno-rtti \
	-m64 \
	-march=x86-64 \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone

override CPPFLAGS := \
	-Iinclude \
	$(CPPFLAGS) \
	-MMD \
	-MP

override LDFLAGS += \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	--no-dynamic-linker \
	-z text \
	-z max-page-size=0x1000 \
	-T linker.ld

override NASMFLAGS += \
	-Wall \
	-f elf64

override CXXFILES := $(shell find -L src -type f -name '*.cpp')
override NASMFILES := $(shell find -L src -type f -name '*.asm')
override OBJ := $(CXXFILES:.cpp=.cpp.o) $(NASMFILES:.asm=.asm.o)
override HEADER_DEPS := $(CXXFILES:.cpp=.cpp.d)

.PHONY: all
all: $(KERNEL)

limine.h:
	curl -Lo $@ https://github.com/limine-bootloader/limine/raw/trunk/limine.h
	mv limine.h include/limine.h

$(KERNEL): GNUmakefile linker.ld $(OBJ)
	echo $(CXXFILES)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

-include $(HEADER_DEPS)

%.cpp.o: %.cpp GNUmakefile limine.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

%.asm.o: %.asm GNUmakefile
	nasm $(NASMFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)

.PHONY: distclean
distclean: clean
	rm -f limine.h
