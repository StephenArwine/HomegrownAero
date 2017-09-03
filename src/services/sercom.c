#include <services.h>

void sercomUSARTInit(const void *const hw, u32_t buad) {

    uint32_t USART_CLKGEN_F = 8000000UL;
    uint64_t br = (uint64_t)65536 * (USART_CLKGEN_F - 16 * buad) / USART_CLKGEN_F;

    SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_ENABLE;
    SYSCTRL->OSC8M.reg -= SYSCTRL_OSC8M_PRESC_3;
    SYSCTRL->OSC8M.reg |= SYSCTRL_OSC8M_ENABLE;

    // GPS pin configs
    ((Port *)PORT)->Group[1].PINCFG[22].reg = 0x41;
    ((Port *)PORT)->Group[1].PINCFG[23].reg = 0x41;
    ((Port *)PORT)->Group[1].PMUX[11].reg = 0x32;


    //enable power to sercom module
    PM->APBCMASK.reg |= PM_APBCMASK_SERCOM5;
    //enable and configure the sercom clock
    GCLK->GENDIV.reg =  GCLK_GENDIV_ID(3) |
                        GCLK_GENDIV_DIV(1);
    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(3) |
                        GCLK_GENCTRL_SRC_OSC8M |
                        GCLK_GENCTRL_IDC |
                        GCLK_GENCTRL_RUNSTDBY |
                        GCLK_GENCTRL_GENEN;
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM5_CORE  |
                        GCLK_CLKCTRL_GEN_GCLK3 |
                        GCLK_CLKCTRL_CLKEN;



    //configure the sercom module for the gps (sercom 5)
    ((Sercom *)hw)->USART.CTRLA.reg = SERCOM_USART_CTRLA_DORD |
                                      SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
                                      SERCOM_USART_CTRLA_RXPO(3) |
                                      SERCOM_USART_CTRLA_TXPO(1);
    USART_sync(hw);



    ((Sercom *)hw)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN
                                      | SERCOM_USART_CTRLB_TXEN
                                      | SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);
    USART_sync(hw);
    ((Sercom *)hw)->USART.BAUD.reg = (uint16_t)br;
    USART_sync(hw);
    ((Sercom *)hw)->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
    USART_sync(hw);

}


inline void sercom_reset(const void *const hw) {
    ((Sercom *)hw)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    while(((Sercom *)hw)->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST);
}

bool _usartByteRecieved(const void *const hw) {
    return _usartGetInteruptRCX(hw);
};



uint8_t _usartGetData(const void *const hw) {
    return ((Sercom *)hw)->USART.DATA.reg;
}

int32_t usartDataRead(const void *const hw, uint8_t *const buf, const uint16_t len) {

    int32_t offset = 0;



        do {
            while (!_usartByteRecieved(hw));
            buf[offset] = _usartGetData(hw);

        } while (++offset < len);
    
    return (int32_t)offset;

}
