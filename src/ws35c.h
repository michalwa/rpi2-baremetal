#ifndef _WS35C_H
#define _WS35C_H

#include <stdint.h>

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
#define WS35C_RESET_PIN  RPI_GPIO_22

// 320x480, vertical orientation
#define WS35C_WIDTH  320
#define WS35C_HEIGHT 480

#define RGB_BLACK rgb(0, 0, 0)
#define RGB_WHITE rgb(0x3F, 0x3F, 0x3F)

typedef struct {
    uint16_t col_start, col_end, page_start, page_end;
} rect_t;

typedef struct {
    uint8_t r, g, b;
} rgb_t;

void ws35c_init(void);
void ws35c_rect_begin(rect_t rect);
void ws35c_rect_end(void);
void ws35c_write(rgb_t rgb);
void ws35c_fill(rect_t rect, rgb_t rgb);

rect_t xywh(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
rgb_t  rgb(uint8_t r, uint8_t g, uint8_t b);

#endif // _WS35C_H
