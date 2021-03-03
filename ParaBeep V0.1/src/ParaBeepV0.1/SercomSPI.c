#include <util.h>



void SPIinit(u8_t id){
		// prevent this clock write from changing any other clocks
		GCLK->PCHCTRL[12].reg = GCLK_PCHCTRL_GEN_GCLK2 | GCLK_PCHCTRL_CHEN;



    sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    while(sercom(id)->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST);

    sercom(id)->SPI.CTRLB.reg
    = SERCOM_SPI_CTRLB_RXEN;

    sercom(id)->SPI.BAUD.reg = 0x00;

    sercom(id)->SPI.CTRLA.reg
    = SERCOM_SPI_CTRLA_ENABLE
    | SERCOM_SPI_CTRLA_MODE(0x3)   
    | SERCOM_SPI_CTRLA_DIPO(0x1)
    | SERCOM_SPI_CTRLA_DOPO(0x3)
    | (0 ? SERCOM_SPI_CTRLA_CPOL : 0)
    | (0 ? SERCOM_SPI_CTRLA_CPHA : 0);

     while  (sercom(id)->SPI.SYNCBUSY.bit.CTRLB);
	
}

u8_t spiDataTransfer(SercomId id, u8_t data) {
	sercom(id)->SPI.DATA.reg = data;
	while(sercom(id)->SPI.INTFLAG.bit.RXC == 0);
	return sercom(id)->SPI.DATA.reg;
}

void spiDataOut(SercomId id, u8_t data) {
	sercom(id)->SPI.DATA.reg = data;
	while(sercom(id)->SPI.INTFLAG.bit.RXC == 0);
	u8_t dummy = sercom(id)->SPI.DATA.reg;
}

u8_t spiDataIn(SercomId id) {
	sercom(id)->SPI.DATA.reg = 0xFF;
	while(sercom(id)->SPI.INTFLAG.bit.RXC == 0);
	return sercom(id)->SPI.DATA.reg;
}