#ifndef _GPIO_H
#define _GPIO_H

// https://github.com/raspberrypi/documentation/blob/f519769a2f246315ad3cf36250d9918e56083769/hardware/raspberrypi/peripheral_addresses.md
#define PERIPHERAL_BASE 0x3F000000
// no fucking clue where this offset is coming from
#define GPIO_BASE       (PERIPHERAL_BASE + 0x200000)
// https://pip-assets.raspberrypi.com/categories/579-raspberry-pi-zero/documents/RP-008249-DS-1-bcm2835-peripherals.pdf
#define GPFSEL4         (GPIO_BASE + 0x10)
#define GPSET1          (GPIO_BASE + 0x20)
#define GPCLR1          (GPIO_BASE + 0x2C)
#define GPIO_INPUT      0x0
#define GPIO_OUTPUT     0x1

#endif // _GPIO_H
