

#include <util.h>
#include <boardDefines.h>


// Configure DFLL in USB recovery mode
const uint32_t dfll_ctrl= SYSCTRL_DFLLCTRL_ENABLE
                          | SYSCTRL_DFLLCTRL_CCDIS
                          | SYSCTRL_DFLLCTRL_BPLCKC
                          | SYSCTRL_DFLLCTRL_ONDEMAND;


static uint32_t cycles_per_ms = 48000000UL / 1000;
static uint32_t cycles_per_us = 48000000UL / 1000000;

#define NVM_DFLL_COARSE_POS    58
#define NVM_DFLL_COARSE_SIZE   6
#define NVM_DFLL_FINE_POS      64
#define NVM_DFLL_FINE_SIZE     10

uint32_t dfll_nvm_val() {
    uint32_t coarse = ( *((uint32_t *)(NVMCTRL_OTP4)
                          + (NVM_DFLL_COARSE_POS / 32))
                        >> (NVM_DFLL_COARSE_POS % 32))
                      & ((1 << NVM_DFLL_COARSE_SIZE) - 1);
    if (coarse == 0x3f) {
        coarse = 0x1f;
    }
    uint32_t fine = ( *((uint32_t *)(NVMCTRL_OTP4)
                        + (NVM_DFLL_FINE_POS / 32))
                      >> (NVM_DFLL_FINE_POS % 32))
                    & ((1 << NVM_DFLL_FINE_SIZE) - 1);
    if (fine == 0x3ff) {
        fine = 0x1ff;
    }

    return SYSCTRL_DFLLVAL_COARSE(coarse) | SYSCTRL_DFLLVAL_FINE(fine);
}

void dfll_wait_for_sync() {
    while (!SYSCTRL->PCLKSR.bit.DFLLRDY);
}

void gclk_enable(uint32_t id, uint32_t src, uint32_t div) {
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(id) | GCLK_GENDIV_DIV(div);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(id) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC(src);
}

void GclkInit(u8_t clk_system) {


    SYSCTRL->INTFLAG.reg = SYSCTRL_INTFLAG_BOD33RDY | SYSCTRL_INTFLAG_BOD33DET |
                           SYSCTRL_INTFLAG_DFLLRDY;
    NVMCTRL->CTRLB.bit.RWS = 2;

    // Initialize GCLK
    PM->APBAMASK.reg |= PM_APBAMASK_GCLK;
    GCLK->CTRL.reg = GCLK_CTRL_SWRST;
    while (GCLK->CTRL.reg & GCLK_CTRL_SWRST);

    // Disable ONDEMAND mode while writing configurations (errata 9905)
    SYSCTRL->DFLLCTRL.reg = dfll_ctrl & ~SYSCTRL_DFLLCTRL_ONDEMAND;
    dfll_wait_for_sync();
    SYSCTRL->DFLLVAL.reg = dfll_nvm_val();
    dfll_wait_for_sync();
    SYSCTRL->DFLLCTRL.reg = dfll_ctrl;


    //For generic clock generator 0, select the DFLL48 Clock as input
    gclk_enable(clk_system, GCLK_SOURCE_DFLL48M, 1);
}

void buzzerInit(){
	
	
  
}

void RtcInit() {

    SYSCTRL->OSC32K.reg = SYSCTRL_OSC32K_ENABLE |
                          SYSCTRL_OSC32K_EN32K |
                          ( 6 << SYSCTRL_OSC32K_STARTUP_Pos);

    SYSCTRL->OSC32K.bit.CALIB =
        ((*(uint32_t *)FUSES_OSC32K_CAL_ADDR >>
          FUSES_OSC32K_CAL_Pos) & 0x7Ful);

    SYSCTRL->OSC32K.reg = SYSCTRL_OSC32K_STARTUP( 0x6u ) | // cf table 15.10 of product datasheet in chapter 15.8.6
                          SYSCTRL_OSC32K_EN32K;
    SYSCTRL->OSC32K.bit.CALIB =
        ((*(uint32_t *)FUSES_OSC32K_CAL_ADDR >>
          FUSES_OSC32K_CAL_Pos) & 0x7Ful);

    SYSCTRL->OSC32K.bit.ENABLE = 1; // separate call, as described in chapter 15.6.3

    while (  (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC32KRDY) == 0 ) {
        // Wait for oscillator stabilization
    }

    GCLK->GENDIV.reg = GCLK_GENDIV_ID(2) | GCLK_GENDIV_DIV(1);

    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(2) |
                        GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSC32K) |
                        GCLK_GENCTRL_IDC |
                        GCLK_GENCTRL_RUNSTDBY |
                        GCLK_GENCTRL_GENEN;
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

// Configure RTC
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(RTC_GCLK_ID) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN(2);

    RTC->MODE1.CTRL.reg = RTC_MODE1_CTRL_MODE_COUNT16;
    while (RTC->MODE1.STATUS.bit.SYNCBUSY);

// Prescaler needs to be enabled separately from the mode for some reason
    RTC->MODE1.CTRL.reg |= RTC_MODE1_CTRL_PRESCALER_DIV32;
    while (RTC->MODE1.STATUS.bit.SYNCBUSY);

    RTC->MODE1.PER.reg = 998;
    while (RTC->MODE1.STATUS.bit.SYNCBUSY);

    RTC->MODE1.READREQ.reg |= RTC_READREQ_RCONT | RTC_READREQ_ADDR(0x10);

    RTC->MODE1.INTENSET.reg = RTC_MODE1_INTENSET_OVF;

    RTC->MODE1.CTRL.bit.ENABLE = 1;
    while (RTC->MODE1.STATUS.bit.SYNCBUSY);

    NVIC_EnableIRQ(RTC_IRQn);
}


void gclkEnable(u32_t id, u32_t src, u32_t div) {
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(id) | GCLK_GENDIV_DIV(div);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(id) | GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC(src);
}


void delayInit(void) {
    cycles_per_ms = 48000000UL;
    cycles_per_ms /= 1000;
    cycles_per_us = cycles_per_ms / 1000;

    NVIC_SetPriority(SysTick_IRQn, 0x0);

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
}

/**
 * \brief Delay loop to delay at least n number of microseconds
 *
 * \param n  Number of microseconds to wait
 */
void delay_us(uint32_t n) {
    while (n--) {
        /* Devide up to blocks of 10u */
        delay_cycles(cycles_per_us);
    }
}


void SysTick_Handler(void) {
    //g_msTicks++;
}

void delay_millis(unsigned ms) {
    unsigned start = g_msTicks;
    while (g_msTicks - start <= ms) {
        __WFI();
    }


}



/**
 * \brief Delay loop to delay at least n number of milliseconds
 *
 * \param n  Number of milliseconds to wait
 */
void delay_ms(uint32_t n) {
    while (n--) {
        /* Devide up to blocks of 1ms */
        delay_cycles(cycles_per_ms);
    }
}


volatile uint32_t time_ms = 0;

void RTC_Handler(void) {
    time_ms += 1000;
    RTC->MODE1.INTFLAG.reg = 0xFF;
}

uint32_t millis(void) {
    uint32_t ms;
    ATOMIC_SECTION_ENTER
    ms = time_ms + RTC->MODE1.COUNT.reg;
    if (RTC->MODE1.INTFLAG.bit.OVF)
        ms = time_ms + RTC->MODE1.COUNT.reg + 1000;
    ATOMIC_SECTION_LEAVE
    return ms;
}

void TC4Init() {
	
	SYSCTRL->OSC8M.reg = SYSCTRL_OSC8M_ENABLE |
						 SYSCTRL_OSC8M_FRANGE_1;
	
	while (  (SYSCTRL->PCLKSR.reg & SYSCTRL_PCLKSR_OSC8MRDY) == 0 ) {
		// Wait for oscillator stabilization
	}
	
	
	GCLK->GENDIV.reg = GCLK_GENDIV_ID(3) | GCLK_GENDIV_DIV(1);

	GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(3) |
						GCLK_GENCTRL_SRC(GCLK_GENCTRL_SRC_OSC8M) |
						GCLK_GENCTRL_IDC |
						GCLK_GENCTRL_RUNSTDBY |
						GCLK_GENCTRL_GENEN;
	while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
	
	
	

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TC4_TC5 |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN(3);
						
	

    PM->APBCMASK.reg |= PM_APBCMASK_TC4;

    TC4->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 |
                            TC_CTRLA_RUNSTDBY |
                            TC_CTRLA_PRESCALER_DIV256;
    TC4->COUNT8.PER.reg = 0x50;

    TC4->COUNT8.INTENSET.reg = TC_INTENSET_OVF;

    TC4->COUNT8.EVCTRL.reg = TC_EVCTRL_OVFEO;

    TC4->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;

    NVIC_EnableIRQ(TC4_IRQn);
}

void TC4_Handler( void ) {
	if (isBuzzing())
	{
    pinToggle(buzzerPin);
	}
	TC4->COUNT8.INTFLAG.reg = 0xFF;

}

void TC5Init() {

    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(TC5_GCLK_ID) |
                        GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN(3);

    PM->APBCMASK.reg |= PM_APBCMASK_TC5;

    TC5->COUNT8.CTRLA.reg = TC_CTRLA_MODE_COUNT8 |
                            TC_CTRLA_RUNSTDBY |
                            TC_CTRLA_PRESCALER_DIV16;

    u16_t logSpeed = 20; //Hz
    u8_t PER_Reg = (long)((32768/16) / logSpeed);

    TC5->COUNT8.PER.reg = PER_Reg;

    TC5->COUNT8.INTENSET.reg = TC_INTENSET_OVF;

    TC5->COUNT8.EVCTRL.reg = TC_EVCTRL_OVFEO;

    TC5->COUNT8.CTRLA.reg |= TC_CTRLA_ENABLE;

    NVIC_EnableIRQ(TC5_IRQn);
}

void TC5_Handler( void ) {
    TC5->COUNT8.INTFLAG.reg = 0xFF;
    writeLog = true;
}


void RTCdelay(u16_t delay){
	uint32_t delayStart = millis();
	
	while(millis()-delayStart < delay){}
}