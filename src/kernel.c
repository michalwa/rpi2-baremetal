#include <stddef.h>
#include <stdint.h>

#include "rpi2b1p1.h"
#include "ws35c.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    (void)r0, (void)r1, (void)atags;

    debug_blink(1);

    ws35c_init();

    debug_blink(1);

    ws35c_fill(9, 49, 9, 49, rgb565(0x13, 0x03, 0x0C));

    for (;;) debug_blink(3);
}
