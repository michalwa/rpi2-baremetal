#include <stddef.h>
#include <stdint.h>

#include "bcm2836.h"
#include "rpi2b1p1.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    (void)r0, (void)r1, (void)atags;

    gpio_fsel(ACTLED_GPIO_PIN, GPIO_OUTPUT);

    uint32_t sleeps[] = { 100, 100, 100, 100, 100, 500 };

    for (;;) {
        for (size_t i = 0; i < sizeof(sleeps) / sizeof(sleeps[0]); i++) {
            gpio_write(ACTLED_GPIO_PIN, (i % 2) ? GPIO_LOW : GPIO_HIGH);
            sleep_ms(sleeps[i]);
        }
    }
}
