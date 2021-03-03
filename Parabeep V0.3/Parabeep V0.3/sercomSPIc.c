/*
 * sercomSPIc.c
 *
 * Created: 2/6/2021 4:42:57 PM
 *  Author: Desktop
 */
#include "util.h"

void SPI0init(u8_t id) {

    enable_clock_generator(1, GCLK_GENCTRL_SRC_DFLL, 12);
    connect_gclk_to_peripheral(1, 7);

    MCLK->APBAMASK.reg |= MCLK_APBAMASK_SERCOM0;


    sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    while(sercom(id)->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST);

    sercom(id)->SPI.CTRLB.reg
        = SERCOM_SPI_CTRLB_RXEN;

    sercom(id)->SPI.BAUD.reg = 0x05;

    sercom(id)->SPI.CTRLA.reg
        = SERCOM_SPI_CTRLA_ENABLE
          | SERCOM_SPI_CTRLA_MODE(0x3)
          | SERCOM_SPI_CTRLA_DIPO(0x2)
          | SERCOM_SPI_CTRLA_DOPO(0x0)
          | SERCOM_SPI_CTRLA_CPOL
          | SERCOM_SPI_CTRLA_CPHA;
          //| (0 ? SERCOM_SPI_CTRLA_CPOL : 0)
          //| (0 ? SERCOM_SPI_CTRLA_CPHA : 0);

    while  (sercom(id)->SPI.SYNCBUSY.bit.CTRLB);

}

void SPI5init(u8_t id) {

    //enable_clock_generator(1, GCLK_GENCTRL_SRC_DFLL, 1);
    connect_gclk_to_peripheral(1, 35);
    MCLK->APBDMASK.reg |= MCLK_APBDMASK_SERCOM5;




    sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
    while(sercom(id)->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST);

    sercom(id)->SPI.CTRLB.reg
        = SERCOM_SPI_CTRLB_RXEN;

    sercom(id)->SPI.BAUD.reg = 0x05;

    sercom(id)->SPI.CTRLA.reg
        = SERCOM_SPI_CTRLA_ENABLE
          | SERCOM_SPI_CTRLA_MODE(0x3)
          | SERCOM_SPI_CTRLA_DIPO(0x3)
          | SERCOM_SPI_CTRLA_DOPO(0x0)
          //| SERCOM_SPI_CTRLA_CPOL
          //| SERCOM_SPI_CTRLA_CPHA;
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