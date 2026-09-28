#ifndef BCM2836_H
#define BCM2836_H

#include <stddef.h>
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
 *   https://wiki.osdev.org/BCM_System_Timer
 */

#define MMIO_BASE    0x3F000000
#define GPIO_BASE    (MMIO_BASE + 0x200000)
#define SPI_BASE     (MMIO_BASE + 0x204000)
#define SYSTIME_BASE (MMIO_BASE + 0x3000)
#define SYSTIME_TPS  1000000
#define UART_BASE    (MMIO_BASE + 0x201000) // there is a missing trailing 0 in the datasheet!
#define UART0_TX_PIN 14
#define UART0_RX_PIN 15

typedef enum {
    GPIO_FSEL_CLEAR = 0x7,
    GPIO_INPUT      = 0x0,
    GPIO_OUTPUT     = 0x1,
    GPIO_ALT_0      = 0x4,
    GPIO_ALT_1      = 0x5,
    GPIO_ALT_2      = 0x6,
    GPIO_ALT_3      = 0x7,
    GPIO_ALT_4      = 0x3,
    GPIO_ALT_5      = 0x2,
} gpio_fsel_t;

typedef enum {
    GPIO_LOW  = 0,
    GPIO_HIGH = 1,
} gpio_state_t;

void gpio_fsel(uint8_t pin, gpio_fsel_t mode);
void gpio_write(uint8_t pin, gpio_state_t state);

typedef enum {
    SPI_TXD      = (1 << 18),
    SPI_RXD      = (1 << 17),
    SPI_DONE     = (1 << 16),
    SPI_REN      = (1 << 12),
    SPI_ADCS     = (1 << 11),
    SPI_TA       = (1 << 7),
    SPI_CLEAR_RX = (1 << 5),
    SPI_CLEAR_TX = (1 << 4),
    SPI_CPOL     = (1 << 3),
    SPI_CPHA     = (1 << 2),
    SPI_CS_0     = 0,
    SPI_CS_1     = 1,
    SPI_CS_2     = 2,
    SPI_CS_NONE  = 3,
    SPI_MODE_0   = 0,
    SPI_MODE_1   = SPI_CPHA,
    SPI_MODE_2   = SPI_CPOL,
    SPI_MODE_3   = (SPI_CPOL | SPI_CPHA),
} spi_ctl_t;

void      spi_cdiv(uint16_t div);
spi_ctl_t spi_ctl_read(void);
void      spi_ctl_write(spi_ctl_t flags);
void      spi_tx_begin(void);
void      spi_tx_write(const uint8_t *buf, size_t sz, size_t count);
void      spi_tx_end(void);

typedef enum {
    UART_RXE    = (1 << 9),
    UART_TXE    = (1 << 8),
    UART_UARTEN = (1 << 0),
} uart_ctl_t;

typedef enum {
    UART_WLEN_8 = (3 << 5),
    UART_WLEN_7 = (2 << 5),
    UART_WLEN_6 = (1 << 5),
    UART_WLEN_5 = (0 << 5),
} uart_wlen_t;

#define UART_FEN (1 << 4)

typedef enum {
    UART_FLAG_TXFF = (1 << 5),
    UART_FLAG_RXFE = (1 << 4),
} uart_flags_t;

// Unable to find any actual source of this information
// https://github.com/raspberrypi/firmware/issues/643
#define FUARTCLK 48000000

void uart0_init(uint32_t baud_rate);
void uart0_putc(char c);
void uart0_puts(const char *s);

void sleep_ms(uint32_t ms);

#endif // BCM2836_H
