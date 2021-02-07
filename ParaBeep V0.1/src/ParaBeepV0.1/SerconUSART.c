#include <util.h>



void USART0init() {


    //float BAUD = 65536*(1-(16*(9600/(16000000/1))));

    GCLK->PCHCTRL[11].reg = GCLK_PCHCTRL_GEN_GCLK2 |
                            GCLK_PCHCTRL_CHEN;
    sercom(0)->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
    while(sercom(0)->USART.CTRLA.reg & SERCOM_USART_CTRLA_SWRST);

    //sercom(0)->USART.BAUD.reg = 57986;
    sercom(0)->USART.BAUD.reg = 64906;
    sercom(0)->USART.CTRLA.reg = SERCOM_USART_CTRLA_ENABLE |
                                 SERCOM_USART_CTRLA_MODE(1) |
                                 SERCOM_USART_CTRLA_RXPO(0) |
                                 SERCOM_USART_CTRLA_TXPO(1) |
                                 SERCOM_USART_CTRLA_DORD;
    sercom(0)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN |
                                 SERCOM_USART_CTRLB_TXEN;
}

void USART2init() {
    //float BAUD = 65536*(1-(16*(9600/(16000000/1))));

    GCLK->PCHCTRL[13].reg = GCLK_PCHCTRL_GEN_GCLK2 |
                            GCLK_PCHCTRL_CHEN;
    sercom(2)->USART.CTRLA.reg = SERCOM_USART_CTRLA_SWRST;
    while(sercom(2)->USART.CTRLA.reg & SERCOM_USART_CTRLA_SWRST);

    sercom(2)->USART.BAUD.reg = 64906;
	//sercom(2)->USART.BAUD.reg = 57986;
    sercom(2)->USART.CTRLA.reg = SERCOM_USART_CTRLA_ENABLE |
                                 SERCOM_USART_CTRLA_MODE(1) |
                                 SERCOM_USART_CTRLA_RXPO(3) |
                                 SERCOM_USART_CTRLA_TXPO(1) |
                                 SERCOM_USART_CTRLA_DORD;
    sercom(2)->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN |
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
