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


void findFlight(Altimeter *my_altimeter) {

    u8_t flightFound = 0;
    u8_t beeps = 1;

    for (u8_t flightAddress = LOGONESTART; flightAddress <= LOGTENEND; flightAddress += 0x06) {

        u8_t addressToCheck[3] = {0x00, 0x00, 0x00};
        AT25SEreadSample(flightAddress, 0x03, addressToCheck);

       volatile u32_t proposedAddress = addressToCheck[0] << 0 | addressToCheck[1] << 8 | addressToCheck[2] << 16;

        if (proposedAddress == 0xFFFFFF) {
            while ( beeps > 0 ) {
                beep(300);
                delay_ms(80);
                --beeps;
            }
            break;
        }
        ++beeps;
    }
}