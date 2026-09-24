#ifndef _WS35C_H
#define _WS35C_H

#include "rpi2b1p1.h"

/*
 * The pin numbers given here seem to match the RPi's board pins
 * (same numbering)
 * https://www.waveshare.com/wiki/3.5inch_RPi_LCD_(C)
 *
 * Still need to map board pin numbers to GPIO numbers, see rpi2b1p1.h
 */

#define WS35C_LCD_RS_PIN RPI_GPIO_18
#define WS35C_LCD_CS_PIN RPI_GPIO_24
#define WS35C_MOSI_PIN   RPI_GPIO_19
#define WS35C_SCLK_PIN   RPI_GPIO_23

void ws35c_init(void);

#endif // _WS35C_H
