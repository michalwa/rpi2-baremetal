#include <stdint.h>

#include "gpio.h"

void spin_wait(uint32_t n) {
    for (volatile uint32_t i = 0; i < n; i++);
}

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    (void)r0, (void)r1, (void)atags;

    // https://github.com/raspberrypi/linux/blob/rpi-6.18.y/arch/arm/boot/dts/broadcom/bcm2836-rpi-2-b.dts
    // https://pip-assets.raspberrypi.com/categories/579-raspberry-pi-zero/documents/RP-008249-DS-1-bcm2835-peripherals.pdf

    uint32_t *gpfsel4 = (uint32_t *)GPFSEL4;
    uint32_t *gpset1 = (uint32_t *)GPSET1;
    uint32_t *gpclr1 = (uint32_t *)GPCLR1;

    // Set pin 47 (ACT LED) as output: bits 23-21 of GPFSEL4
    *gpfsel4 = (*gpfsel4 & ~(0x7 << 21)) | (GPIO_OUTPUT << 21);

    for (;;) {
        // Set pin 47 (bit 15 of GPSET1), writing a 0 bit has no effect
        *gpset1 = 1 << 15;

        spin_wait(0x100000);

        // Clear pin 47 (bit 15 of GPCLR1)
        *gpclr1 = 1 << 15;

        spin_wait(0x200000);
    }
}
