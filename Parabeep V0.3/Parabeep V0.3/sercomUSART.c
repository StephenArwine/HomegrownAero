#include <util.h>


void USART7init() {


    connect_gclk_to_peripheral(1, 37);
    MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM7;


    //float BAUD = 65536*(1-(16*(9600/(16000000/1))));

    sercom(7)->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
    while(sercom(7)->USART.CTRLA.reg & SERCOM_USART_CTRLA_SWRST);

    //sercom(0)->USART.BAUD.reg = 57986;
    sercom(7)->USART.BAUD.reg = 65326;
    sercom(7)->USART.CTRLA.reg = SERCOM_USART_CTRLA_ENABLE |
                                 SERCOM_USART_CTRLA_MODE(1) |
                                 SERCOM_USART_CTRLA_RXPO(1) |
                                 SERCOM_USART_CTRLA_TXPO(0) |
                                 SERCOM_USART_CTRLA_DORD;
    sercom(7)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN |
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