#ifndef _RPI2B1P1_H
#define _RPI2B1P1_H

#include <stdint.h>

/*
 * Mapping from board pin numbers to GPIO numbers
 * https://pinout.xyz
 */
#define RPI_GPIO_3  2
#define RPI_GPIO_5  3
#define RPI_GPIO_7  4
#define RPI_GPIO_8  14
#define RPI_GPIO_10 15
#define RPI_GPIO_11 17
#define RPI_GPIO_12 18
#define RPI_GPIO_13 27
#define RPI_GPIO_15 22
#define RPI_GPIO_16 23
#define RPI_GPIO_18 24
#define RPI_GPIO_19 10
#define RPI_GPIO_21 9
#define RPI_GPIO_22 25
#define RPI_GPIO_23 11
#define RPI_GPIO_24 8
#define RPI_GPIO_26 7
#define RPI_GPIO_27 0
#define RPI_GPIO_28 1
#define RPI_GPIO_29 5
#define RPI_GPIO_31 6
#define RPI_GPIO_32 12
#define RPI_GPIO_33 13
#define RPI_GPIO_35 19
#define RPI_GPIO_36 16
#define RPI_GPIO_37 26
#define RPI_GPIO_38 20
#define RPI_GPIO_40 21

#define ACTLED_GPIO_PIN 47

void debug_blink(uint8_t n);

#endif // _RPI2B1P1_H
