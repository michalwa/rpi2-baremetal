#include "rpi2b1p1.h"

#include "bcm2836.h"

void debug_blink(uint8_t n) {
    gpio_fsel(ACTLED_GPIO_PIN, GPIO_OUTPUT);

    while (n--) {
        gpio_write(ACTLED_GPIO_PIN, GPIO_HIGH);
        sleep_ms(100);
        gpio_write(ACTLED_GPIO_PIN, GPIO_LOW);
        sleep_ms(100);
    }

    gpio_write(ACTLED_GPIO_PIN, GPIO_LOW);
    sleep_ms(500);
}
