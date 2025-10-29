OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
    driver_header PT_LOAD FLAGS((1 << 3)); /* special flag so the loader knows where to find the header */
    text PT_LOAD;
    data PT_LOAD;
    rodata PT_LOAD;
    dynamic PT_DYNAMIC;
}

SECTIONS
{
    .driver_header : {
        *(.driver_header .driver_header.*)
    } :driver_header

    .text : {
        *(.text .text.*)
    } :text

    .data : {
        *(.data .data.*)
    } :data

    .rodata : {
        *(.rodata .rodata.*)
    } :rodata

    .dynamic : {
        *(.dynamic)
    } :data :dynamic

    .bss : {
        *(.bss .bss.*)
        *(COMMON)
    } :data

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}