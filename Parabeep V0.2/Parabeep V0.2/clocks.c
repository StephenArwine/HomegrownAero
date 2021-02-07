#include "util.h"


static uint32_t cycles_per_ms = 48000000UL / 1000;
static uint32_t cycles_per_us = 48000000UL / 1000000;

void delayInit(void) {
    NVIC_SetPriority(SysTick_IRQn, 0x0);

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    TC_INTENSET_OVF |
                    SysTick_CTRL_ENABLE_Msk;
}

static inline void delay_cycles(const uint32_t n) {
    if (n > 0) {
        SysTick->LOAD = n;
        SysTick->VAL = 0;
        SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {};

        SysTick->CTRL = 0;

    }
}

void delay_ms(uint32_t n) {
    while (n--) {
        /* divide up to blocks of 1ms */
        delay_cycles(cycles_per_ms);
    }
}

void delay_us(uint32_t n) {
    while (n--) {
        /* divide up to blocks of 10u */
        delay_cycles(cycles_per_us);
    }
}