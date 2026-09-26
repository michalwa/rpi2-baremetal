#include <stddef.h>
#include <stdint.h>

#include "rpi2b1p1.h"
#include "ws35c.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    (void)r0, (void)r1, (void)atags;

    debug_blink(1);

    ws35c_init();
    ws35c_fill(0, WS35C_COL_MAX, 0, WS35C_PAGE_MAX, rgb(0, 0, 0));
    ws35c_fill(9, 49, 9, 49, rgb(0x23, 0x03, 0x1C));
    ws35c_fill(59, 129, 9, 159, rgb(0x3F, 0x3F, 0x3F));

    ws35c_fill(9, 49, 209, 249, rgb(0x3F, 0, 0));
    ws35c_fill(59, 99, 209, 249, rgb(0, 0x3F, 0));
    ws35c_fill(109, 149, 209, 249, rgb(0, 0, 0x3F));

    for (;;) debug_blink(3);
}
