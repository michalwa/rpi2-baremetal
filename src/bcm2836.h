#ifndef _BCM2836_H
#define _BCM2836_H

#include <stdint.h>

/*
 * The Raspberry Pi 2 model B v1.1 uses the Broadcom BCM2836 processor, which is
 * similar to BCM2835 and apparently shares most characteristics, so resources
 * related to the BCM2835 are also relevant. BCM2836 uses the Cortex-A7
 * microarchitecture.
 *
 * Resources:
 *   https://github.com/raspberrypi/documentation/blob/f519769a2f246315ad3cf36250d9918e56083769/hardware/raspberrypi/peripheral_addresses.md
 *   https://pip-assets.raspberrypi.com/categories/579-raspberry-pi-zero/documents/RP-008249-DS-1-bcm2835-peripherals.pdf
 */

#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000)

typedef enum {
    GPIO_FSEL_CLEAR = 0x7,
    GPIO_INPUT      = 0x0,
    GPIO_OUTPUT     = 0x1,
} gpio_fsel_t;

typedef enum {
    GPIO_LOW  = 0,
    GPIO_HIGH = 1,
} gpio_state_t;

void gpio_fsel(uint8_t pin, gpio_fsel_t mode);
void gpio_write(uint8_t pin, gpio_state_t state);

#endif // _BCM2836_H
