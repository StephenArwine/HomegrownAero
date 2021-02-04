#include <util.h>

static uint32_t cycles_per_ms = 16000000UL / 1000;
static uint32_t cycles_per_us = 16000000UL / 1000000;


void GclkInit(u8_t clk_system) {
    OSCCTRL->OSC16MCTRL.reg = OSCCTRL_OSC16MCTRL_FSEL_16 |
                              OSCCTRL_OSC16MCTRL_ENABLE;
    while(!OSCCTRL->STATUS.bit.OSC16MRDY);
    GCLK->GENCTRL[0].reg = GCLK_GENCTRL_SRC_OSC16M |
                           GCLK_GENCTRL_DIV(0) |
                           GCLK_GENCTRL_GENEN;
    while(GCLK->SYNCBUSY.bit.GENCTRL0);

}

u32_t buzzerFreq = 50;

void setBuzzerFreq(u32_t freq) {

    buzzerFreq = ((16000000/1)/16)/freq;

    TC2->COUNT16.PERBUF.reg = buzzerFreq;
    TC2->COUNT16.CTRLBSET.reg = TC_CTRLBSET_CMD_UPDATE;


}


void TC2Init() {

    GCLK->GENCTRL[2].reg = GCLK_GENCTRL_SRC_OSC16M |
                           GCLK_GENCTRL_DIV(1) |
                           GCLK_GENCTRL_GENEN;
    while(GCLK->SYNCBUSY.bit.GENCTRL2);
    GCLK->PCHCTRL[15].reg = GCLK_PCHCTRL_GEN_GCLK2 |
                            GCLK_PCHCTRL_CHEN;

    TC2->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while(TC2->COUNT16.SYNCBUSY.bit.SWRST);
    TC2->COUNT16.INTENSET.reg = TC_INTENSET_OVF_Msk;
    TC2->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 |
                             TC_CTRLA_PRESCALER_DIV16 |
                             TC_CTRLA_ENABLE;
    TC2->COUNT16.PER.reg = buzzerFreq;


    while(TC2->COUNT16.SYNCBUSY.bit.ENABLE);

    NVIC_EnableIRQ(TC2_IRQn);

}

volatile u16_t TC0Dur = 1000;

void TC0Init() {

    GCLK->GENCTRL[3].reg = GCLK_GENCTRL_SRC_OSCULP32K |
                           GCLK_GENCTRL_DIV(32) |
                           GCLK_GENCTRL_GENEN;
    while(GCLK->SYNCBUSY.bit.GENCTRL3);
    GCLK->PCHCTRL[14].reg = GCLK_PCHCTRL_GEN_GCLK3 |
                            GCLK_PCHCTRL_CHEN;

    TC0->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while(TC0->COUNT16.SYNCBUSY.bit.SWRST);
    //TC0->COUNT16.CTRLBSET.reg = TC_CTRLBSET_ONESHOT;
    TC0->COUNT16.INTENSET.reg = TC_INTENSET_OVF_Msk;
    TC0->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 |
                             TC_CTRLA_PRESCALER_DIV1 |
                             TC_CTRLA_ENABLE;
    TC0->COUNT16.PER.reg = TC0Dur;

    while(TC0->COUNT16.SYNCBUSY.bit.ENABLE);
    NVIC_EnableIRQ(TC0_IRQn);
}

void TC1Init() {

    //GCLK->GENCTRL[4].reg = GCLK_GENCTRL_SRC_OSCULP32K | GCLK_GENCTRL_DIV(32) | GCLK_GENCTRL_GENEN;
    //while(GCLK->SYNCBUSY.bit.GENCTRL4);
    //GCLK->PCHCTRL[14].reg = GCLK_PCHCTRL_GEN_GCLK4 | GCLK_PCHCTRL_CHEN;

    TC1->COUNT8.CTRLA.reg = TC_CTRLA_SWRST;
    while(TC1->COUNT8.SYNCBUSY.bit.SWRST);
    TC1->COUNT8.INTENSET.reg = TC_INTENSET_OVF_Msk;
    TC1->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 |
                            TC_CTRLA_PRESCALER_DIV1 |
                            TC_CTRLA_ENABLE;
    TC1->COUNT8.PER.reg = 50;

    while(TC1->COUNT8.SYNCBUSY.bit.ENABLE);
    NVIC_EnableIRQ(TC1_IRQn);
}


void rtcInit() {
    RTC->MODE1.CTRLA.reg = RTC_MODE1_CTRLA_SWRST;
    while (RTC->MODE1.SYNCBUSY.bit.SWRST);

    OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_ULP32K;

    RTC->MODE1.INTENSET.reg = RTC_MODE1_INTENSET_OVF_Msk;

    RTC->MODE1.CTRLA.reg = RTC_MODE1_CTRLA_MODE_COUNT16 |
                           RTC_MODE1_CTRLA_PRESCALER_DIV32 |
                           RTC_MODE1_CTRLA_COUNTSYNC;
    RTC->MODE1.PER.reg = RTC_MODE1_PER_PER(998);
    RTC->MODE1.CTRLA.reg |= RTC_MODE1_CTRLA_ENABLE;
    while (RTC->MODE1.SYNCBUSY.bit.ENABLE);


    NVIC_EnableIRQ(RTC_IRQn);
}


void delayInit(void) {
    NVIC_SetPriority(SysTick_IRQn, 0x0);

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    TC_INTENSET_OVF |
                    SysTick_CTRL_ENABLE_Msk;
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

volatile uint32_t time_ms = 0;

uint32_t millis(void) {
    uint32_t ms;
    ATOMIC_SECTION_ENTER
    ms = time_ms + RTC->MODE1.COUNT.reg;
    if (RTC->MODE1.INTFLAG.bit.OVF) {
        //ms = time_ms + RTC->MODE1.COUNT.reg + 1000;
    }
    ATOMIC_SECTION_LEAVE
    return ms;
}

void TC0_Handler(void) {

    buzzing = !buzzing;

    if (!buzzing) {
        pinLow(BuzzerPin);
    }

    TC0->COUNT16.INTFLAG.reg = 0xFF;
}

void TC1_Handler(void) {

    takeSample = true;
    TC1->COUNT8.INTFLAG.reg = 0xFF;

}

void TC2_Handler(void) {
    if (buzzing) {
        pinToggle(BuzzerPin);
    }
    TC2->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF_Msk;
}

void RTC_Handler(void) {
    time_ms += 1000;
    RTC->MODE1.INTFLAG.reg = RTC_MODE1_INTFLAG_OVF_Msk;
}

void RTCdelay(u16_t delay) {
    uint32_t delayStart = millis();

    while(millis()-delayStart < delay) {}
}


