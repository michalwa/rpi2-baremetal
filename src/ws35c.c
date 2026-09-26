#include "ws35c.h"

#include <stdint.h>

#include "bcm2836.h"

/*
 * Waveshare 3.5inch RPi LCD (C)
 *   https://www.waveshare.com/wiki/3.5inch_RPi_LCD_(C)
 *
 * ILI9486 controller datasheet:
 *   https://www.lcdwiki.com/res/MAR3501/datasheet_ILI9486.pdf
 *
 * Community sources:
 *   https://github.com/notro/fbtft/blob/master/fb_ili9486.c
 *   https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_Drivers/ILI9486_Init.h
 *   https://github.com/sonicpp/ili9486
 *   https://github.com/khiyamiftikhar/esp-lcd-ili9486
 */

// Because of some shift register shenanigans, every single byte sent to the
// board must be preceded by a 0 byte, except RGB666 data bytes for some reason
static inline void spi_tx_write16(uint8_t b) {
    spi_tx_write(0);
    spi_tx_write(b);
}

static inline void data_begin(void) {
    gpio_write(WS35C_LCD_RS_PIN, GPIO_HIGH);
    spi_tx_begin();
}

static inline void data_end(void) {
    spi_tx_end();

    gpio_write(WS35C_LCD_CS_PIN, GPIO_HIGH);
}

static inline void command(uint8_t cmd) {
    gpio_write(WS35C_LCD_RS_PIN, GPIO_LOW);
    gpio_write(WS35C_LCD_CS_PIN, GPIO_LOW);

    spi_tx_begin();
    spi_tx_write16(cmd);
    spi_tx_end();
}

static inline void command1(uint8_t cmd, uint8_t arg) {
    command(cmd);

    data_begin();
    spi_tx_write16(arg);
    data_end();
}

void ws35c_init(void) {
    // Enable SPI functions for GPIO pins
    gpio_fsel(WS35C_LCD_RS_PIN, GPIO_OUTPUT);
    gpio_fsel(WS35C_LCD_CS_PIN, GPIO_OUTPUT); // drive this manually instead of SPI_CE0_N
    gpio_fsel(WS35C_MOSI_PIN, GPIO_ALT_0);    // SPI0_MOSI
    gpio_fsel(WS35C_SCLK_PIN, GPIO_ALT_0);    // SPI0_SCLK

    // Hardware reset pulse
    gpio_fsel(WS35C_RESET_PIN, GPIO_OUTPUT);
    gpio_write(WS35C_RESET_PIN, GPIO_LOW);
    sleep_ms(20);
    gpio_write(WS35C_RESET_PIN, GPIO_HIGH);
    sleep_ms(150);

    // The BCM2836 core clock runs at 250Mhz (see config.txt),
    // we want at most 125Mhz
    spi_cdiv(2);
    spi_ctl_write(SPI_MODE_0 | SPI_CS_NONE);

    command(0x11); // sleep OUT
    sleep_ms(120); // wait while the controller does a self-diagnostic check

    command1(0x36, 0x48); // memory access control: MX (orientation), BGR
    // interface pixel format: 18 bits per pixel
    // Despite 16 bits per pixel (0x55) being a valid config, ILI9486 allegedly
    // does not support it when writing over SPI
    // https://github.com/khiyamiftikhar/esp-lcd-ili9486#3%EF%B8%8F%E2%83%A3-rgb666-required-over-spi
    command1(0x3A, 0x66); // interface pixel format: 18 bits per pixel
    command1(0xC2, 0x55); // power control 3: step-up circuit frequencies

    command(0xC5); // VCOM control 1
    data_begin();
    {
        spi_tx_write16(0x00);
        spi_tx_write16(0x00);
        spi_tx_write16(0x00);
        spi_tx_write16(0x00);
    }
    data_end();

    command(0xE0); // Positive Gamma Control
    data_begin();
    {
        spi_tx_write16(0x0F); // these configure some voltages
        spi_tx_write16(0x1F);
        spi_tx_write16(0x1C);
        spi_tx_write16(0x0C);
        spi_tx_write16(0x0F);
        spi_tx_write16(0x08);
        spi_tx_write16(0x48);
        spi_tx_write16(0x98);
        spi_tx_write16(0x37);
        spi_tx_write16(0x0A);
        spi_tx_write16(0x13);
        spi_tx_write16(0x04);
        spi_tx_write16(0x11);
        spi_tx_write16(0x0D);
        spi_tx_write16(0x00);
    }
    data_end();

    command(0xE1); // Negative Gamma Correction
    data_begin();
    {
        spi_tx_write16(0x0F);
        spi_tx_write16(0x32);
        spi_tx_write16(0x2E);
        spi_tx_write16(0x0B);
        spi_tx_write16(0x0D);
        spi_tx_write16(0x05);
        spi_tx_write16(0x47);
        spi_tx_write16(0x75);
        spi_tx_write16(0x37);
        spi_tx_write16(0x06);
        spi_tx_write16(0x10);
        spi_tx_write16(0x03);
        spi_tx_write16(0x24);
        spi_tx_write16(0x20);
        spi_tx_write16(0x00);
    }
    data_end();

    command(0x29); // display ON
    sleep_ms(50);
}

void ws35c_fill(
    uint16_t col_start, uint16_t col_end, uint16_t page_start, uint16_t page_end, rgb_t rgb
) {
    command(0x2A); // Column Address Set
    data_begin();
    {
        spi_tx_write16(col_start >> 8);
        spi_tx_write16(col_start & 0xFF);
        spi_tx_write16(col_end >> 8);
        spi_tx_write16(col_end & 0xFF);
    }
    data_end();

    command(0x2B); // Page Address Set
    data_begin();
    {
        spi_tx_write16(page_start >> 8);
        spi_tx_write16(page_start & 0xFF);
        spi_tx_write16(page_end >> 8);
        spi_tx_write16(page_end & 0xFF);
    }
    data_end();

    uint32_t size = ((uint32_t)col_end - col_start + 1) * (page_end - page_start + 1);

    command(0x2C); // Memory Write
    data_begin();
    {
        for (uint32_t i = 0; i < size; i++) {
            // Apparently the 6-bit components have to be shifted such that they
            // occupy the 6 MSBs
            // https://github.com/khiyamiftikhar/esp-lcd-ili9486/blob/7dcb034e9bd059ce8a34929762b6bdc361538c18/src/esp_ili9486_panel.c#L43-L53
            spi_tx_write(rgb.r << 2);
            spi_tx_write(rgb.g << 2);
            spi_tx_write(rgb.b << 2);
        }
    }
    data_end();
}

inline rgb_t rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (rgb_t){ r, g, b };
}
