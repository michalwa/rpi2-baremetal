#include "psf.h"

inline psf2_font_t psf2_init(const void *start) {
    psf2_font_t font = (psf2_font_t)start;

    if (font->magic != PSF2_MAGIC)
        for (;;);

    return font;
}

inline psf2_glyph_t psf2_glyph(psf2_font_t font, uint32_t i) {
    return (psf2_glyph_t)font + font->header_size + i * font->glyph_size;
}

int psf2_pixel(psf2_font_t font, psf2_glyph_t glyph, uint8_t col, uint8_t row) {
    uint8_t bytes_per_row = (font->glyph_width + 7) / 8;
    uint8_t data          = glyph[row * bytes_per_row + col / 8];

    return (data >> (7 - col % 8)) & 1;
}
