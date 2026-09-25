#include "ws35c.h"

#include "bcm2836.h"

/*
 * Waveshare 3.5inch RPi LCD (C)
 *
 *   https://www.waveshare.com/wiki/3.5inch_RPi_LCD_(C)
 *
 * The display supposedly uses the ILI9486 controller. This is not officially
 * documented anywhere.
 *
 * Datasheet:
 *   https://www.lcdwiki.com/res/MAR3501/datasheet_ILI9486.pdf
 *
 * Community sources:
 *   https://github.com/notro/fbtft/blob/master/fb_ili9486.c
 *   https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_Drivers/ILI9486_Init.h
 *   https://github.com/sonicpp/ili9486
 *
 *   https://github.com/swkim01/waveshare-dtoverlays/blob/4b5fcbd3aaeaec2d1089bd6cb083db9984f13ea2/waveshare35c.dts#L67-L81
 */

static inline void data_begin(void) {
    gpio_write(WS35C_LCD_RS_PIN, GPIO_HIGH);
    spi_tx_begin();
}

static inline void data_end(void) {
    spi_tx_end();
}

static inline void command(uint8_t cmd) {
    gpio_write(WS35C_LCD_RS_PIN, GPIO_LOW);
    spi_tx_begin();
    spi_tx_write(cmd);
    spi_tx_end();
}

static inline void command1(uint8_t cmd, uint8_t arg) {
    gpio_write(WS35C_LCD_RS_PIN, GPIO_LOW);
    spi_tx_begin();
    spi_tx_write(cmd);
    spi_tx_end();

    data_begin();
    spi_tx_write(arg);
    data_end();
}

void ws35c_init(void) {
    // Enable SPI functions for GPIO pins
    gpio_fsel(WS35C_LCD_RS_PIN, GPIO_OUTPUT);
    gpio_fsel(WS35C_MOSI_PIN, GPIO_ALT0);   // SPI0_MOSI
    gpio_fsel(WS35C_LCD_CS_PIN, GPIO_ALT0); // SPI0_CE0_N
    gpio_fsel(WS35C_SCLK_PIN, GPIO_ALT0);   // SPI0_SCLK

    // The BCM2836 core clock runs at 250Mhz (see config.txt),
    // we want at most 125Mhz
    spi_cdiv(4);                            // TODO: try different values
    spi_cs_write(SPI_MODE_0 & ~SPI_CS_REN); // clearing SPI_CS_REN is redundant but left for clarity

    command1(0xB0, 0x00); // interface mode control: it takes some flags, just clear them
    command(0x11);        // sleep OUT
    sleep_ms(250);        // wait while the controller does a self-diagnostic check
    command1(0x3A, 0x55); // interface pixel format: 16 bits per pixel
    command1(0xC2, 0x44); // power control 3: set step-up cycles to 2H and 8H (no idea)

    command(0xC5); // VCOM control 1
    data_begin();
    {
        spi_tx_write(0x00);
        spi_tx_write(0x00);
        spi_tx_write(0x00);
        spi_tx_write(0x00);
    }
    data_end();

    command(0xE0); // Positive Gamma Control
    data_begin();
    {
        spi_tx_write(0x0F); // these configure some voltages
        spi_tx_write(0x1F);
        spi_tx_write(0x1C);
        spi_tx_write(0x0C);
        spi_tx_write(0x0F);
        spi_tx_write(0x08);
        spi_tx_write(0x48);
        spi_tx_write(0x98);
        spi_tx_write(0x37);
        spi_tx_write(0x0A);
        spi_tx_write(0x13);
        spi_tx_write(0x04);
        spi_tx_write(0x11);
        spi_tx_write(0x0D);
        spi_tx_write(0x00);
    }
    data_end();

    command(0xE1); // Negative Gamma Correction
    data_begin();
    {
        spi_tx_write(0x0F);
        spi_tx_write(0x32);
        spi_tx_write(0x2E);
        spi_tx_write(0x0B);
        spi_tx_write(0x0D);
        spi_tx_write(0x05);
        spi_tx_write(0x47);
        spi_tx_write(0x75);
        spi_tx_write(0x37);
        spi_tx_write(0x06);
        spi_tx_write(0x10);
        spi_tx_write(0x03);
        spi_tx_write(0x24);
        spi_tx_write(0x20);
        spi_tx_write(0x00);
    }
    data_end();

    command(0xE2); // Digital Gamma Control 1
    data_begin();
    {
        spi_tx_write(0x0F);
        spi_tx_write(0x32);
        spi_tx_write(0x2E);
        spi_tx_write(0x0B);
        spi_tx_write(0x0D);
        spi_tx_write(0x05);
        spi_tx_write(0x47);
        spi_tx_write(0x75);
        spi_tx_write(0x37);
        spi_tx_write(0x06);
        spi_tx_write(0x10);
        spi_tx_write(0x03);
        spi_tx_write(0x24);
        spi_tx_write(0x20);
        spi_tx_write(0x00);
    }
    data_end();

    command(0x11); // sleep OUT
    command(0x29); // display ON
}

void ws35c_fill(
    uint16_t col_start, uint16_t col_end, uint16_t page_start, uint16_t page_end, uint16_t rgb
) {
    command(0x2A); // Column Address Set
    data_begin();
    {
        spi_tx_write(col_start >> 8);
        spi_tx_write(col_start & 0xFF);
        spi_tx_write(col_end >> 8);
        spi_tx_write(col_end & 0xFF);
    }
    data_end();

    command(0x2B); // Page Address Set
    data_begin();
    {
        spi_tx_write(page_start >> 8);
        spi_tx_write(page_start & 0xFF);
        spi_tx_write(page_end >> 8);
        spi_tx_write(page_end & 0xFF);
    }
    data_end();

    uint32_t size = ((uint32_t)col_end - col_start + 1) * (page_end - page_start + 1);

    command(0x2C); // Memory Write
    data_begin();
    {
        for (uint32_t i = 0; i < size; i++) {
            spi_tx_write(rgb >> 8);
            spi_tx_write(rgb & 0xFF);
        }
    }
    data_end();
}

inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
}
