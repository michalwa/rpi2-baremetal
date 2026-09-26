#ifndef _PSF_H
#define _PSF_H

#include <stdint.h>

/*
 * https://wiki.osdev.org/PC_Screen_Font
 *
 * Assume we're always in little-endian and both PSF versions use LE, so we
 * don't have to worry about endianness
 */

#define PSF2_MAGIC 0x864AB572

typedef enum {
    PSF2_HAS_UNICODE_TABLE = 1,
} psf2_flags_t;

typedef struct {
    uint32_t magic;   // PSF2_MAGIC
    uint32_t version; // 0
    uint32_t header_size;
    uint32_t flags; // psf2_flags_t
    uint32_t num_glyphs;
    uint32_t glyph_size;
    uint32_t glyph_height;
    uint32_t glyph_width;
} psf2_header_t;

typedef const psf2_header_t *psf2_font_t;

typedef const uint8_t *psf2_glyph_t;

psf2_font_t  psf2_init(const void *start);
psf2_glyph_t psf2_glyph(psf2_font_t font, uint32_t i);
int          psf2_pixel(psf2_font_t font, psf2_glyph_t glyph, uint8_t col, uint8_t row);

#endif // _PSF_H
