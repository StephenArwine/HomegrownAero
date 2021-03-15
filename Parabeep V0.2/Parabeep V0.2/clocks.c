
#include "clocks.h"


static uint32_t cycles_per_ms = 48000000UL / 1000;
static uint32_t cycles_per_us = 48000000UL / 1000000;


bool gclk_enabled(uint8_t gclk) {
    return GCLK->GENCTRL[gclk].bit.GENEN;
}

void disable_gclk(uint8_t gclk) {
    while ((GCLK->SYNCBUSY.vec.GENCTRL & (1 << gclk)) != 0) {}
    GCLK->GENCTRL[gclk].bit.GENEN = false;
    while ((GCLK->SYNCBUSY.vec.GENCTRL & (1 << gclk)) != 0) {}
}

void connect_gclk_to_peripheral(uint8_t gclk, uint8_t peripheral) {
    GCLK->PCHCTRL[peripheral].reg = GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN(gclk);
    while(GCLK->SYNCBUSY.reg != 0) {}
}

void disconnect_gclk_from_peripheral(uint8_t gclk, uint8_t peripheral) {
    GCLK->PCHCTRL[peripheral].reg = 0;
}

static void enable_clock_generator_sync(uint8_t gclk, uint32_t source, uint16_t divisor, bool sync) {
    uint32_t divsel = 0;
    // The datasheet says 8 bits and max value of 512, how is that possible?
    if (divisor > 255) { // Generator 1 has 16 bits
        divsel = GCLK_GENCTRL_DIVSEL;
        for (int i = 15; i > 0; i--) {
            if (divisor & (1 << i)) {
                divisor = i - 1;
                break;
            }
        }
    }

    GCLK->GENCTRL[gclk].reg = GCLK_GENCTRL_SRC(source) | GCLK_GENCTRL_DIV(divisor) | divsel | GCLK_GENCTRL_OE | GCLK_GENCTRL_GENEN;
    if (sync)
        while ((GCLK->SYNCBUSY.vec.GENCTRL & (1 << gclk)) != 0) {}
}

void init_clock_source_osculp32k(void) {
    // Calibration value is loaded at startup
    OSC32KCTRL->OSCULP32K.bit.EN1K = 0;
    OSC32KCTRL->OSCULP32K.bit.EN32K = 1;
}

void enable_clock_generator(uint8_t gclk, uint32_t source, uint16_t divisor) {
    enable_clock_generator_sync(gclk, source, divisor, true);
}

void disable_clock_generator(uint8_t gclk) {
    GCLK->GENCTRL[gclk].reg = 0;
    while ((GCLK->SYNCBUSY.vec.GENCTRL & (1 << gclk)) != 0) {}
}

static void init_clock_source_dpll0(void) {
    GCLK->PCHCTRL[OSCCTRL_GCLK_ID_FDPLL0].reg = GCLK_PCHCTRL_CHEN | GCLK_PCHCTRL_GEN(5);
    OSCCTRL->Dpll[0].DPLLRATIO.reg = OSCCTRL_DPLLRATIO_LDRFRAC(0) | OSCCTRL_DPLLRATIO_LDR(59);
    OSCCTRL->Dpll[0].DPLLCTRLB.reg = OSCCTRL_DPLLCTRLB_REFCLK(0);
    OSCCTRL->Dpll[0].DPLLCTRLA.reg = OSCCTRL_DPLLCTRLA_ENABLE;

    while (!(OSCCTRL->Dpll[0].DPLLSTATUS.bit.LOCK || OSCCTRL->Dpll[0].DPLLSTATUS.bit.CLKRDY)) {}
}

volatile u8_t TC0Dur = 50;

void TC0Init() {

    enable_clock_generator(4, GCLK_GENCTRL_SRC_OSCULP32K, 32);

    connect_gclk_to_peripheral(4, 9);

    MCLK->APBAMASK.reg |= MCLK_APBAMASK_TC0;

    TC0->COUNT8.CTRLA.reg = TC_CTRLA_SWRST;
    while(TC0->COUNT8.SYNCBUSY.bit.SWRST);

    TC0->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 |
                            TC_CTRLA_PRESCALER_DIV1 |
                            TC_CTRLA_ENABLE;
    TC0->COUNT8.PERBUF.reg = TC0Dur;

    while(TC0->COUNT8.SYNCBUSY.bit.ENABLE);
    NVIC_EnableIRQ(TC0_IRQn);
}

void TC0_enable_interupt() {
    TC0->COUNT8.INTENSET.reg = TC_INTENSET_OVF;
}

void TC0_disable_interupt() {
    TC0->COUNT8.INTENCLR.reg = TC_INTENCLR_OVF;
}


volatile u8_t TC2Dur = 50;

void TC2Init() { //buzzer

    enable_clock_generator(3, GCLK_GENCTRL_SRC_DFLL, 32);

    connect_gclk_to_peripheral(3, 26);

    MCLK->APBBMASK.reg |= MCLK_APBBMASK_TC2;

    TC2->COUNT8.CTRLA.reg = TC_CTRLA_SWRST;
    while(TC2->COUNT8.SYNCBUSY.bit.SWRST);
    //TC0->COUNT16.CTRLBSET.reg = TC_CTRLBSET_ONESHOT;
    TC2->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 |
                            TC_CTRLA_PRESCALER_DIV4 |
                            TC_CTRLA_ENABLE;
    TC2->COUNT8.PERBUF.reg = TC2Dur;

    while(TC2->COUNT8.SYNCBUSY.bit.ENABLE);
    NVIC_EnableIRQ(TC2_IRQn);
}

void TC2_enable_interupt() { //buzzer
    TC2->COUNT8.INTENSET.reg = TC_INTENSET_OVF;
}

void TC2_disable_interupt() { //buzzer
    TC2->COUNT8.INTENCLR.reg = TC_INTENCLR_OVF;
    pinLow(BuzzerPin);
}


void TC0_Handler(void) {
    TC0->COUNT8.INTFLAG.reg = TC_INTFLAG_OVF;
    sample.takeSample = true;
}


void TC2_Handler(void) {
    TC2->COUNT8.INTFLAG.reg = TC_INTFLAG_OVF;
    pinToggle(BuzzerPin);
}



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

void rtcInit() {
    RTC->MODE1.CTRLA.reg = RTC_MODE1_CTRLA_SWRST;
    while (RTC->MODE1.SYNCBUSY.bit.SWRST);

    OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_ULP32K;

    RTC->MODE1.INTENSET.reg = RTC_MODE1_INTENSET_OVF;

    RTC->MODE1.CTRLA.reg = RTC_MODE1_CTRLA_MODE_COUNT16 |
                           RTC_MODE1_CTRLA_PRESCALER_DIV32 |
                           RTC_MODE1_CTRLA_COUNTSYNC;
    RTC->MODE1.PER.reg = RTC_MODE1_PER_PER(998);
    RTC->MODE1.CTRLA.reg |= RTC_MODE1_CTRLA_ENABLE;
    while (RTC->MODE1.SYNCBUSY.bit.ENABLE);


    NVIC_EnableIRQ(RTC_IRQn);
}

volatile uint32_t time_ms = 0;

void RTC_Handler(void) {
    RTC->MODE1.INTFLAG.reg = RTC_MODE1_INTFLAG_OVF;
    time_ms += 1000;
}

uint32_t millis(void) {
    uint32_t ms;
    ATOMIC_SECTION_ENTER
    ms = time_ms + RTC->MODE1.COUNT.reg;
    if (RTC->MODE1.INTFLAG.bit.OVF) {
        ms = time_ms + RTC->MODE1.COUNT.reg + 1000;
    }
    ATOMIC_SECTION_LEAVE
    return ms;
}