#include "bcm2836.h"

static inline uint32_t *gpfsel(uint8_t pin) {
    return (uint32_t *)(GPIO_BASE + (pin / 10) * sizeof(uint32_t));
}

static inline uint8_t gpfsel_bit_offset(uint8_t pin) {
    // A single 32-bit FSEL register holds 10 3-bit fields (bits 0..29)
    return (pin % 10) * 3;
}

static inline uint32_t *gpset(uint8_t pin) {
    return (uint32_t *)(GPIO_BASE + 0x1C + (pin / 32) * sizeof(uint32_t));
}

static inline uint32_t *gpclr(uint8_t pin) {
    return (uint32_t *)(GPIO_BASE + 0x28 + (pin / 32) * sizeof(uint32_t));
}

static inline uint8_t gpsetclr_bit(uint8_t pin) {
    return pin % 32;
}

void gpio_fsel(uint8_t pin, gpio_fsel_t mode) {
    uint32_t *fsel = gpfsel(pin);
    uint8_t   bit  = gpfsel_bit_offset(pin);

    *fsel = (*fsel & ~(GPIO_FSEL_CLEAR << bit)) | (mode << bit);
}

void gpio_write(uint8_t pin, gpio_state_t state) {
    *(state ? gpset(pin) : gpclr(pin)) = 1 << gpsetclr_bit(pin);
}

static inline int spi_tx_ready(void) {
    return *(volatile uint32_t *)SPI_BASE & (1 << 18);
}

static inline int spi_tx_done(void) {
    return *(volatile uint32_t *)SPI_BASE & (1 << 16);
}

void spi_cdiv(uint16_t div) {
    *(uint32_t *)(SPI_BASE + 0x8) = div;
}

void spi_tx_begin(void) {
    *(uint32_t *)SPI_BASE |= (1 << 7);
}

void spi_tx_end(void) {
    while (!spi_tx_done());
    *(uint32_t *)SPI_BASE &= ~(1 << 7);
}

void spi_tx_write(uint8_t byte) {
    while (!spi_tx_ready());
    *(uint32_t *)(SPI_BASE + 0x4) = byte;
}

static inline uint64_t systime(void) {
    uint32_t lo = *(volatile uint32_t *)(SYSTIME_BASE + 0x4);
    uint32_t hi = *(volatile uint32_t *)(SYSTIME_BASE + 0x8);

    return ((uint64_t)hi << 32) | lo;
}

void sleep_ms(uint32_t ms) {
    uint64_t end = systime() + ms * SYSTIME_TPS / 1000;

    while (systime() < end);
}
