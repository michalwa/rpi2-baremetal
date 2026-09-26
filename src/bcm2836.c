#include "bcm2836.h"

// 32-bit MMIO register
typedef volatile uint32_t reg32_t;

static inline reg32_t *gpfsel(uint8_t pin) {
    return (reg32_t *)(GPIO_BASE + (pin / 10) * sizeof(reg32_t));
}

static inline uint8_t gpfsel_bit_offset(uint8_t pin) {
    // A single 32-bit FSEL register holds 10 3-bit fields (bits 0..29)
    return (pin % 10) * 3;
}

static inline reg32_t *gpset(uint8_t pin) {
    return (reg32_t *)(GPIO_BASE + 0x1C + (pin / 32) * sizeof(reg32_t));
}

static inline reg32_t *gpclr(uint8_t pin) {
    return (reg32_t *)(GPIO_BASE + 0x28 + (pin / 32) * sizeof(reg32_t));
}

static inline uint8_t gpsetclr_bit(uint8_t pin) {
    return pin % 32;
}

void gpio_fsel(uint8_t pin, gpio_fsel_t mode) {
    reg32_t *fsel = gpfsel(pin);
    uint8_t  bit  = gpfsel_bit_offset(pin);

    *fsel = (*fsel & ~(GPIO_FSEL_CLEAR << bit)) | (mode << bit);
}

void gpio_write(uint8_t pin, gpio_state_t state) {
    *(state ? gpset(pin) : gpclr(pin)) = 1 << gpsetclr_bit(pin);
}

inline void spi_cdiv(uint16_t div) {
    *(reg32_t *)(SPI_BASE + 0x8) = div;
}

inline spi_ctl_t spi_ctl_read(void) {
    return *(reg32_t *)SPI_BASE;
}

inline void spi_ctl_write(spi_ctl_t flags) {
    *(reg32_t *)SPI_BASE = flags;
}

inline void spi_tx_begin(void) {
    *(reg32_t *)SPI_BASE |= SPI_TA;
}

// https://github.com/torvalds/linux/blob/165768bb70265b5c38cf0b73fafd75be235f8b14/drivers/spi/spi-bcm2835.c#L997-L1050
void spi_tx_write(uint8_t byte) {
    while (!(spi_ctl_read() & SPI_TXD));
    *(reg32_t *)(SPI_BASE + 0x4) = byte;

    // Read back bytes to free up the FIFO in case slaves respond
    uint32_t rx;
    while (spi_ctl_read() & SPI_RXD) rx = *(reg32_t *)(SPI_BASE + 0x4);
    (void)rx;
}

void spi_tx_end(void) {
    while (!(spi_ctl_read() & SPI_DONE));

    // https://github.com/torvalds/linux/blob/165768bb70265b5c38cf0b73fafd75be235f8b14/drivers/spi/spi-bcm2835.c#L358-L366
    spi_ctl_t cs = spi_ctl_read();
    cs &= ~SPI_TA;
    cs |= SPI_DONE | SPI_CLEAR_RX | SPI_CLEAR_TX;
    spi_ctl_write(cs);
}

static inline uint64_t systime(void) {
    uint32_t lo = *(reg32_t *)(SYSTIME_BASE + 0x4);
    uint32_t hi = *(reg32_t *)(SYSTIME_BASE + 0x8);

    return ((uint64_t)hi << 32) | lo;
}

void sleep_ms(uint32_t ms) {
    uint64_t end = systime() + ms * SYSTIME_TPS / 1000;

    while (systime() < end);
}
