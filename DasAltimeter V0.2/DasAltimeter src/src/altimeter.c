#include <util.h>
#include <boardDefines.h>


void IMUinit() {

    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, BMI_ACCEL_PMU_RANGE);
    dummy_rx = spiDataTransfer(SPI0, BMI_ACCEL_16G_RANGE);
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


bool unplugged() {
    if (sample.voltage.batFloat < 3.5) {
        return false;
    }
}
