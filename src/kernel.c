#include <stdint.h>

#include "bcm2836.h"
#include "rpi2b1p1.h"

void spin_wait(uint32_t n) {
    for (volatile uint32_t i = 0; i < n; i++);
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    (void)r0, (void)r1, (void)atags;

    gpio_fsel(ACTLED_GPIO_PIN, GPIO_OUTPUT);

    for (;;) {
        gpio_write(ACTLED_GPIO_PIN, GPIO_HIGH);
        spin_wait(0x100000);
        gpio_write(ACTLED_GPIO_PIN, GPIO_LOW);
        spin_wait(0x200000);
    }
}
