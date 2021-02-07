/*
 * sercomSPIc.c
 *
 * Created: 2/6/2021 4:42:57 PM
 *  Author: Desktop
 */
#include "util.h"

void SPIinit(u8_t id) {

//     GCLK->PCHCTRL[23].reg = GCLK_PCHCTRL_GEN_GCLK0 | GCLK_PCHCTRL_CHEN;
//     MCLK->APBAMASK.reg = MCLK_APBBMASK_SERCOM2;
//     //PM->APBCMASK.reg |= 1 << (MCLK_APBCMASK_SERCOM0_Pos + id);
// 
// 
// 
//     sercom(id)->SPI.CTRLA.reg = SERCOM_SPI_CTRLA_SWRST;
//     while(sercom(id)->SPI.CTRLA.reg & SERCOM_SPI_CTRLA_SWRST);
// 
//     sercom(id)->SPI.CTRLB.reg
//         = SERCOM_SPI_CTRLB_RXEN;
// 
//     sercom(id)->SPI.BAUD.reg = 0x00;
// 
//     sercom(id)->SPI.CTRLA.reg
//         = SERCOM_SPI_CTRLA_ENABLE
//           | SERCOM_SPI_CTRLA_MODE(0x3)
//           | SERCOM_SPI_CTRLA_DIPO(0x3)
//           | SERCOM_SPI_CTRLA_DOPO(0x0)
//           | (0 ? SERCOM_SPI_CTRLA_CPOL : 0)
//           | (0 ? SERCOM_SPI_CTRLA_CPHA : 0);
// 
//     while  (sercom(id)->SPI.SYNCBUSY.bit.CTRLB);

}