#include <util.h>
#include <boardDefines.h>


void IMUinit() {

    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, 0x0F);
    dummy_rx = spiDataTransfer(SPI0, 0x0C);
    pinHigh(cs_accel);

}


