#include <util.h>

void sercomClockEnable(SercomId id, uint32_t clock_channel, u8_t divider) {
    // prevent this clock write from changing any other clocks
    PM->APBCMASK.reg |= 1 << (PM_APBCMASK_SERCOM0_Pos + id);


    if (clock_channel != 0) {
        // clock generators 3-8 have 8 division factor bits - DIV[7:0]
        gclkEnable(clock_channel, GCLK_SOURCE_DFLL48M, divider);
    }


    // attach clock
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |
                        GCLK_CLKCTRL_GEN(clock_channel) |
                        GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE + id);
}


void sercomReset(SercomId id) {
    sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    while(sercom(id)->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST);
}


void sercomSpiSlaveInit(SercomId id, u32_t dipo, u32_t dopo, bool cpol, bool cpha) {
    sercomReset(id);
    sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_SLAVE;


    sercom(id)->SPI.CTRLB.reg
        = SERCOM_SPI_CTRLB_RXEN
          | SERCOM_SPI_CTRLB_SSDE
          | SERCOM_SPI_CTRLB_PLOADEN;

    sercom(id)->SPI.CTRLA.reg
        = SERCOM_SPI_CTRLA_ENABLE
          | SERCOM_SPI_CTRLA_MODE_SPI_SLAVE
          | SERCOM_SPI_CTRLA_DIPO(dipo)
          | SERCOM_SPI_CTRLA_DOPO(dopo)
          | (cpol ? SERCOM_SPI_CTRLA_CPOL : 0)
          | (cpha ? SERCOM_SPI_CTRLA_CPHA : 0);
}


void sercomSpiMasterInit(SercomId id, u32_t dipo, u32_t dopo, bool cpol, bool cpha, u8_t baud) {
	
	//sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_ENABLE
	
    sercomReset(id);
	
    sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_MODE_SPI_MASTER;


    sercom(id)->SPI.CTRLB.reg
        = SERCOM_SPI_CTRLB_RXEN
          | SERCOM_SPI_CTRLB_SSDE;

    SPI_sync(id);

    sercom(id)->SPI.BAUD.reg = baud;

    SPI_sync(id);


    sercom(id)->SPI.CTRLA.reg
        = SERCOM_SPI_CTRLA_ENABLE
          | SERCOM_SPI_CTRLA_MODE_SPI_MASTER
          | SERCOM_SPI_CTRLA_DIPO(dipo)
          | SERCOM_SPI_CTRLA_DOPO(dopo)
          | (cpol ? SERCOM_SPI_CTRLA_CPOL : 0)
          | (cpha ? SERCOM_SPI_CTRLA_CPHA : 0);

    SPI_sync(id);



}


void sercomI2cMasterInit(SercomId id, u8_t baud) {
    sercomReset(id);
    sercom(id)->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;
    sercom(id)->I2CM.BAUD.reg = baud;
    sercom(id)->I2CM.CTRLA.reg
        = SERCOM_I2CM_CTRLA_ENABLE
          | SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;
    sercom(id)->I2CM.STATUS.reg = SERCOM_I2CM_STATUS_BUSSTATE(1);
}


void sercomUartInit(SercomId id, u32_t rxpo, u32_t txpo, u32_t baud) {
    sercomReset(id);
    sercom(id)->USART.CTRLA.reg = SERCOM_USART_CTRLA_MODE_USART_INT_CLK;
    sercom(id)->USART.BAUD.reg = baud;
    sercom(id)->USART.CTRLB.reg
        = SERCOM_USART_CTRLB_RXEN
          | SERCOM_USART_CTRLB_TXEN;
    sercom(id)->USART.CTRLA.reg
        = SERCOM_USART_CTRLA_ENABLE
          | SERCOM_USART_CTRLA_MODE_USART_INT_CLK
          | SERCOM_SPI_CTRLA_DORD
          | SERCOM_USART_CTRLA_TXPO(txpo)
          | SERCOM_USART_CTRLA_RXPO(rxpo);
}
