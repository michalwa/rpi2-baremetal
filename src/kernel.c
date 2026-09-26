#include <stddef.h>
#include <stdint.h>

#include "bcm2836.h"
#include "max7219.h"
#include "rpi2b1p1.h"
// #include "ws35c.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    (void)r0, (void)r1, (void)atags;

    // debug_blink(1);
    // ws35c_init();
    // debug_blink(1);
    // ws35c_fill(9, 49, 9, 49, rgb565(0x13, 0x03, 0x0C));

    max7219_init();

    debug_blink(3);

    for (uint8_t j = 0; j < 37; j++) {
        for (uint8_t i = 1; i < 9; i++)
            max7219_write(i, max7219_digits[j + 1][i - 1], i, max7219_digits[j][i - 1]);
        debug_blink(1);
        sleep_ms(1000);
    }

    for (;;) debug_blink(3);
}
