// Enter your pattern code here and click on the Play button
// below to execute your code. The results will then be visible
// in the Pattern Data view.
// 
// More information can be found in the documentation.
// 
// 
// Simple example:
// 
// import std.io;
// 
// struct Pattern {
//     u32 int;
//     float floating_point;
// };
// 
// Pattern my_pattern @ 0x00;
// std::print("0x{:08X}", my_pattern.int);

struct Psf2Header {
    u32 magic;   // PSF2_MAGIC
    u32 version; // 0
    u32 header_size;
    u32 flags; // psf2_flags_t
    u32 num_glyphs;
    u32 glyph_size;
    u32 glyph_height;
    u32 glyph_width;
};

Psf2Header header @ 0;