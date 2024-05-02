#define LIMINE_COMMON_MAGIC 0xc7b1dd30df4c8b88, 0x0a82e883a194f07b

#define LIMINE_FRAMEBUFFER_REQUEST [ LIMINE_COMMON_MAGIC, 0x9d5827dcd881dd75, 0xa3148604f6fab11b ]

#define LIMINE_FRAMEBUFFER_RGB 1

struct limine_video_mode {
    pitch: i64;
    width: i64;
    height: i64;
    bpp: i16;
    memory_model: i8;
    red_mask_size: i8;
    red_mask_shift: i8;
    green_mask_size: i8;
    green_mask_shift: i8;
    blue_mask_size: i8;
    blue_mask_shift: i8;
}

struct limine_framebuffer {
    address: void*;
    width: i64;
    height: i64;
    pitch: i64;
    bpp: i16;
    memory_model: i8;
    red_mask_size: i8;
    red_mask_shift: i8;
    green_mask_size: i8;
    green_mask_shift: i8;
    blue_mask_size: i8;
    blue_mask_shift: i8;
    unused: i8[7];
    edid_size: i64;
    edid: void*;
    mode_count: i64;
    modes: struct limine_video_mode**;
}

struct limine_framebuffer_response {
    revision: i64;
    framebuffer_count: i64;
    framebuffers: struct limine_framebuffer**;
}

struct limine_framebuffer_request {
    id: i64[4];
    revision: i64;
    response: struct limine_framebuffer_response*;
}