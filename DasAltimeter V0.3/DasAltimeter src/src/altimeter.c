#include <util.h>
#include <boardDefines.h>

#include "samd21.h"

/* Initialize segments */
extern uint32_t _sfixed;
extern uint32_t _efixed;
extern uint32_t _etext;
extern uint32_t _srelocate;
extern uint32_t _erelocate;
extern uint32_t _szero;
extern uint32_t _ezero;
extern uint32_t _sstack;
extern uint32_t _estack;


void IMUinit() {

    pinLow(cs_accel);
    spiDataOut(SPI0, BMI_ACCEL_PMU_RANGE);
    spiDataOut(SPI0, BMI_ACCEL_16G_RANGE);
    pinHigh(cs_accel);

}

bool USARTconnectionAvaliable() {

    if (sercom(USART3)->SPI.INTFLAG.bit.RXC == 1) {
        u8_t connectAttempt = usartDataIn(USART3);
        if (connectAttempt == 0x48) {
            usartDataOut(USART3, 'H');
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void sendTheasePagesToComputer(u32_t firstPage, u32_t lastPage) {

    u16_t pagesToSend = (lastPage - firstPage) >> 8;

    u32_t pageToReadAddress = firstPage;

    for (u16_t page = 0; page <= pagesToSend; ++page) {

        u8_t data[256];
        AT25SEreadPage(pageToReadAddress , data);

        for (u16_t dataByte = 0; dataByte < 256; ++dataByte) {
            usartDataOut(USART3, data[dataByte]);
        }

        pageToReadAddress = pageToReadAddress + 0x100;
    }
}

int32_t altitudeAGL() {
    return altitude - offsets.groundOffset;
}


bool unplugged() {
    if (sample.voltage.batFloat < 3.4) {
        return true;
    } else {
        return false;
    }
}

void shutDown() {


    delay_ms(100);
    TC4->COUNT8.CTRLA.reg = 0;
    TC5->COUNT8.CTRLA.reg = 0;
    NVIC_DisableIRQ(TC4_IRQn);
    NVIC_DisableIRQ(TC5_IRQn);
    NVIC_DisableIRQ(RTC_IRQn);
    RTC->MODE1.CTRL.bit.ENABLE = 0;
    SysTick->CTRL = 0;
    PM->APBAMASK.reg = 0;

    //turn off sercom clocks
    //GCLK->CLKCTRL.reg

    GCLK->GENDIV.reg  = (GCLK_GENDIV_DIV(1)  | GCLK_GENDIV_ID(0));
    GCLK->GENCTRL.reg = (GCLK_GENCTRL_ID(0)  | (GCLK_GENCTRL_SRC_OSC8M) | (GCLK_GENCTRL_GENEN));

    pinHigh(LedPin);

//     uint32_t *pSrc, *pDest;
//
//     /* Initialize the relocate segment */
//     pSrc = &_etext;
//     pDest = &_srelocate;
//
//     if (pSrc != pDest) {
//         for (; pDest < &_erelocate;) {
//             *pDest++ = *pSrc++;
//         }
//     }
//
//     /* Clear the zero segment */
//     for (pDest = &_szero; pDest < &_ezero;) {
//         *pDest++ = 0;
//     }
//
//     /* Set the vector table base address */
//     pSrc = (uint32_t *) & _sfixed;
//     SCB->VTOR = ((uint32_t) pSrc & SCB_VTOR_TBLOFF_Msk);
//
//     /* Change default QOS values to have the best performance*/
//     SBMATRIX->SFR[SBMATRIX_SLAVE_HMCRAMC0].reg = 2;
//     DMAC->QOSCTRL.bit.DQOS = 2;
//     DMAC->QOSCTRL.bit.FQOS = 2;
//     DMAC->QOSCTRL.bit.WRBQOS = 2;
//
//     /* Overwriting the default value of the NVMCTRL.CTRLB.MANW bit (errata reference 13134) */
//     NVMCTRL->CTRLB.bit.MANW = 1;
//
//     /* Initialize the C library */
//     __libc_init_array();

    SYSCTRL->BOD33.reg = SYSCTRL_BOD33_PSEL_DIV8 | SYSCTRL_BOD33_ACTION_RESET | SYSCTRL_BOD33_LEVEL(60);

    SYSCTRL->BOD33.reg |= SYSCTRL_BOD33_ENABLE;
    while(1) {

    }
}

void finishFlight() {

    flightState = flightIdle;
    AT25SFHoldTillReady();
    writeFlightEndAddress( );
    unpluggedJingle();
    shutDown();

}

void finishFromPad() {
    unpluggedJingle();
    shutDown();
}


void POST() {
    uint8_t dummy_Tx = 0xFF;

    bool postFailed = false;
    u8_t failType = 0;

    //Baro post first
    u8_t crc = MS5803_CRC4();
    if (crc != coefficients_[7]) {
        postFailed = true;
        failType = 1;
    }

    //Memory Post
    pinLow(cs_mem);
    spiDataOut(SPI1,0x9f); // read id and mfg code
    u8_t mfgID = spiDataIn(SPI1);
    u8_t deviceID1 = spiDataIn(SPI1);
    u8_t deviceID2 = spiDataIn(SPI1);
    pinHigh(cs_mem);

    if ( (mfgID != 0x1f) & (deviceID1 != 0x86) & (deviceID2 != 0x01)) {
        postFailed = true;
        failType = 2;
    }

    //Accelerometer Post
    pinLow(cs_accel);
    spiDataOut(SPI0, BMI055_BGW_CHIPID | BMI055_READ_REG);
    u8_t accelID = spiDataTransfer(SPI0,dummy_Tx);
    pinHigh(cs_accel);

    if (accelID != 0xFA) {
        postFailed = true;
        failType = 3;
    }

    //Gyro Post
    pinLow(cs_gyro);
    spiDataOut(SPI0, BMI055_BGW_CHIPID | BMI055_READ_REG);
    u8_t gyroID = spiDataTransfer(SPI0,dummy_Tx);
    pinHigh(cs_gyro);

    if (gyroID != 0x0F) {
        postFailed = true;
        failType = 4;
    }

    //Analog Accelerometer Post


    if (postFailed) {
        while (1) {
            u8_t beepCount;

            for (beepCount = failType; beepCount > 0; beepCount--) {
                beep(300);
                delay_ms(300);
            }
            delay_ms(1500);
        }
    }
}
