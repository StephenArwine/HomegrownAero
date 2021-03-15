#include <util.h>


void USART2init() {

    enable_clock_generator(2, GCLK_GENCTRL_SRC_DFLL, 1);

    connect_gclk_to_peripheral(2, 23);
    MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM2;


    //float BAUD = 65536*(1-(16*(9600/(16000000/1))));

    sercom(2)->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
    while(sercom(2)->USART.CTRLA.reg & SERCOM_USART_CTRLA_SWRST);

    //sercom(0)->USART.BAUD.reg = 57986;
    sercom(2)->USART.BAUD.reg = 63018;
    sercom(2)->USART.CTRLA.reg = SERCOM_USART_CTRLA_ENABLE |
                                 SERCOM_USART_CTRLA_MODE(1) |
                                 SERCOM_USART_CTRLA_RXPO(1) |
                                 SERCOM_USART_CTRLA_TXPO(0) |
                                 SERCOM_USART_CTRLA_DORD;

    //SERCOM_USART_CTRLA_DORD;
    sercom(2)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN |
                                 SERCOM_USART_CTRLB_TXEN;
}

void USART3init() {

    enable_clock_generator(2, GCLK_GENCTRL_SRC_DFLL, 1);


    connect_gclk_to_peripheral(2, 24);
    MCLK->APBBMASK.reg |= MCLK_APBBMASK_SERCOM3;


    //float BAUD = 65536*(1-(16*(9600/(16000000/1))));

    sercom(3)->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
    while(sercom(3)->USART.CTRLA.reg & SERCOM_USART_CTRLA_SWRST);

    //sercom(0)->USART.BAUD.reg = 57986;
    sercom(3)->USART.BAUD.reg = 63018;
    //sercom(3)->USART.BAUD.reg = 65326;

    sercom(3)->USART.CTRLA.reg = SERCOM_USART_CTRLA_ENABLE |
                                 SERCOM_USART_CTRLA_MODE(1) |
                                 SERCOM_USART_CTRLA_RXPO(1) |
                                 SERCOM_USART_CTRLA_TXPO(0) |
                                 SERCOM_USART_CTRLA_DORD;

    sercom(3)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN |
                                 SERCOM_USART_CTRLB_TXEN;
}


void usartDataOut(SercomId id, u8_t data) {
    sercom(id)->USART.DATA.reg = data;
    //this needs to be enabled normally!!!!!!!
    while(sercom(id)->USART.INTFLAG.bit.TXC == 0);
}

u8_t usartDataIn(SercomId id) {
    return sercom(id)->USART.DATA.reg;
    //this needs to be enabled normally!!!!!!!
    while(sercom(id)->USART.INTFLAG.bit.RXC == 0);
}