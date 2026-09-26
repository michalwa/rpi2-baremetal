#include <stddef.h>
#include <stdint.h>

#include "psf.h"
#include "rpi2b1p1.h"
#include "ws35c.h"

extern const uint8_t _binary_res_cozette_psf_start[];

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    (void)r0, (void)r1, (void)atags;

    debug_blink(1);

    ws35c_init();
    ws35c_fill(xywh(0, 0, WS35C_WIDTH, WS35C_HEIGHT), RGB_BLACK);

    psf2_font_t font = psf2_init(_binary_res_cozette_psf_start);

#define CELL_WIDTH  6
#define CELL_HEIGHT 13
#define TEXT        "wassup world"
#define COLS        (WS35C_WIDTH / CELL_WIDTH)

    for (uint32_t i = 0; i < sizeof(TEXT) - 1; i++) {
        psf2_glyph_t glyph = psf2_glyph(font, TEXT[i]);

        uint32_t glyph_row = i / COLS;
        uint32_t glyph_col = i % COLS;

        ws35c_rect_begin(xywh(
            glyph_col * CELL_WIDTH, glyph_row * CELL_HEIGHT, font->glyph_width, font->glyph_height
        ));

        for (uint8_t row = 0; row < font->glyph_height; row++)
            for (uint8_t col = 0; col < font->glyph_width; col++)
                ws35c_write(psf2_pixel(font, glyph, col, row) ? RGB_WHITE : RGB_BLACK);

        ws35c_rect_end();
    }

    for (;;) debug_blink(1);
}
