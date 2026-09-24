#include "ws35c.h"

#include "bcm2836.h"

typedef enum {
    WS35C_COMMAND = GPIO_LOW,
    WS35C_DATA    = GPIO_HIGH,
} ws35c_rs_t;

static void ws35c_write(ws35c_rs_t rs, uint8_t value) {
    gpio_write(WS35C_LCD_RS_PIN, (gpio_state_t)rs);

    spi_tx_begin();
    spi_tx_write(value);
    spi_tx_end();
}

void ws35c_init(void) {
    // Enable SPI functions for GPIO pins
    gpio_fsel(WS35C_LCD_RS_PIN, GPIO_OUTPUT);
    gpio_fsel(WS35C_MOSI_PIN, GPIO_ALT0);   // SPI0_MOSI
    gpio_fsel(WS35C_LCD_CS_PIN, GPIO_ALT0); // SPI0_CE0_N
    gpio_fsel(WS35C_SCLK_PIN, GPIO_ALT0);   // SPI0_SCLK

    // The BCM2836 core clock runs at 250Mhz by default, we want at most 125Mhz
    spi_cdiv(2);

    /*
     * The display supposedly uses the ILI9486 controller. This is not
     * officially documented anywhere.
     *
     * Datasheet:
     *   https://www.lcdwiki.com/res/MAR3501/datasheet_ILI9486.pdf
     *
     * Community sources:
     *   https://github.com/notro/fbtft/blob/master/fb_ili9486.c
     *   https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_Drivers/ILI9486_Init.h
     *   https://github.com/sonicpp/ili9486
     */

    ws35c_write(WS35C_COMMAND, 0xB0); // interface mode control
    ws35c_write(WS35C_DATA, 0x00);    // it takes some flags, just clear them

    ws35c_write(WS35C_COMMAND, 0x11); // sleep OUT
    sleep_ms(250);                    // wait while the controller does a self-diagnostic check

    ws35c_write(WS35C_COMMAND, 0x3A); // interface pixel format
    ws35c_write(WS35C_DATA, 0x55);    // 16 bits per pixel

    ws35c_write(WS35C_COMMAND, 0xC2); // power control 3
    ws35c_write(WS35C_DATA, 0x44);    // set step-up cycles to 2H and 8H (no idea)

    ws35c_write(WS35C_COMMAND, 0xC5); // VCOM control 1
    ws35c_write(WS35C_DATA, 0x00);
    ws35c_write(WS35C_DATA, 0x00);
    ws35c_write(WS35C_DATA, 0x00);
    ws35c_write(WS35C_DATA, 0x00);

    ws35c_write(WS35C_COMMAND, 0xE0); // Positive Gamma Control
    ws35c_write(WS35C_DATA, 0x0F);    // these values configure voltages
    ws35c_write(WS35C_DATA, 0x1F);
    ws35c_write(WS35C_DATA, 0x1C);
    ws35c_write(WS35C_DATA, 0x0C);
    ws35c_write(WS35C_DATA, 0x0F);
    ws35c_write(WS35C_DATA, 0x08);
    ws35c_write(WS35C_DATA, 0x48);
    ws35c_write(WS35C_DATA, 0x98);
    ws35c_write(WS35C_DATA, 0x37);
    ws35c_write(WS35C_DATA, 0x0A);
    ws35c_write(WS35C_DATA, 0x13);
    ws35c_write(WS35C_DATA, 0x04);
    ws35c_write(WS35C_DATA, 0x11);
    ws35c_write(WS35C_DATA, 0x0D);
    ws35c_write(WS35C_DATA, 0x00);

    ws35c_write(WS35C_COMMAND, 0xE1); // Negative Gamma Correction
    ws35c_write(WS35C_DATA, 0x0F);
    ws35c_write(WS35C_DATA, 0x32);
    ws35c_write(WS35C_DATA, 0x2E);
    ws35c_write(WS35C_DATA, 0x0B);
    ws35c_write(WS35C_DATA, 0x0D);
    ws35c_write(WS35C_DATA, 0x05);
    ws35c_write(WS35C_DATA, 0x47);
    ws35c_write(WS35C_DATA, 0x75);
    ws35c_write(WS35C_DATA, 0x37);
    ws35c_write(WS35C_DATA, 0x06);
    ws35c_write(WS35C_DATA, 0x10);
    ws35c_write(WS35C_DATA, 0x03);
    ws35c_write(WS35C_DATA, 0x24);
    ws35c_write(WS35C_DATA, 0x20);
    ws35c_write(WS35C_DATA, 0x00);

    ws35c_write(WS35C_COMMAND, 0xE2); // Digital Gamma Control 1
    ws35c_write(WS35C_DATA, 0x0F);
    ws35c_write(WS35C_DATA, 0x32);
    ws35c_write(WS35C_DATA, 0x2E);
    ws35c_write(WS35C_DATA, 0x0B);
    ws35c_write(WS35C_DATA, 0x0D);
    ws35c_write(WS35C_DATA, 0x05);
    ws35c_write(WS35C_DATA, 0x47);
    ws35c_write(WS35C_DATA, 0x75);
    ws35c_write(WS35C_DATA, 0x37);
    ws35c_write(WS35C_DATA, 0x06);
    ws35c_write(WS35C_DATA, 0x10);
    ws35c_write(WS35C_DATA, 0x03);
    ws35c_write(WS35C_DATA, 0x24);
    ws35c_write(WS35C_DATA, 0x20);
    ws35c_write(WS35C_DATA, 0x00);

    ws35c_write(WS35C_COMMAND, 0x11); // sleep OUT
    ws35c_write(WS35C_COMMAND, 0x29); // display ON
}
