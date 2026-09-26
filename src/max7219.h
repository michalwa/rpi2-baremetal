#ifndef _MAX7219_H
#define _MAX7219_H

#include <stdint.h>

#include "rpi2b1p1.h"

#define MAX7219_CS_PIN RPI_GPIO_24
#define MAX7219_MOSI_PIN RPI_GPIO_19
#define MAX7219_SCLK_PIN RPI_GPIO_23

extern uint8_t max7219_digits[38][8];

void max7219_init(void);
/*
 * I suspect there is a latch that switches between the two controllers on the
 * 16x8 dot matrix board, so `addr1, data1` likely goes to controller 1 and
 * `addr2, data2` goes to controller 2.
 *
 * The datasheet for MAX7219 documents the serial data format as 16-bit, where
 * the address is actually 4 bits, the high nibble is ignored.
 */
void max7219_write(uint8_t addr1, uint8_t data1, uint8_t addr2, uint8_t data2);

#endif // _MAX7219_H
