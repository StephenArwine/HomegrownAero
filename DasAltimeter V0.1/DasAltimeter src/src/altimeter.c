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

    volatile u8_t lastFlightFound = 0;
    u8_t beeps = 0;

    for (u16_t flightAddress = FLIGHTZEROSTART; flightAddress <= FLIGHTTENSTART; flightAddress += 0x03) {

        ++beeps;

        u8_t addressToCheck[3] = {0x00, 0x00, 0x00};
        AT25SEreadSample(flightAddress, 0x03, addressToCheck);

        u32_t proposedAddress = addressToCheck[0] << 0 | addressToCheck[1] << 8 | addressToCheck[2] << 16;

        if (proposedAddress == 0xFFFFFF) {
            while ( beeps > 0 ) {
                beep(400);
                delay_ms(200);
                --beeps;
            }
            delay_ms(500);
            u32_t previousFlightEnd = findFlightEnding(my_altimeter, lastFlightFound);
            my_altimeter->myFlashMemory.currentAddress = previousFlightEnd;
            beep(1000);
            delay_ms(200);
            beep(1000);
            break;
        }
        ++lastFlightFound;
    }
}

u32_t findFlightEnding(Altimeter *my_altimeter, u8_t lastFlightFound) {

    u32_t flightToSearch = (lastFlightFound * 0x03) + (FLIGHTZEROSTART - 0x03);

    u8_t searchStartByte[3] = {0x00, 0x00, 0x00};
    AT25SEreadSample(flightToSearch, 0x03, searchStartByte);

    u32_t searchStart = searchStartByte[0] << 0 | searchStartByte[1] << 8 | searchStartByte[2] << 16;

    u32_t byteToCheckAddress = searchStart;

    int16_t sensorsFound = 0;
    int16_t flightSampFound = 0;

    bool flightEndFound = false;
    while (!flightEndFound) {

        u8_t byteToCheck = AT25SFGetByte(byteToCheckAddress);

        if (byteToCheck == 0x41) {
            byteToCheckAddress += 24;
            ++sensorsFound;
        } else if (byteToCheck == 0x46) {
            byteToCheckAddress += 17;
            ++flightSampFound;
        } else if (byteToCheck == 0xFF) {
            return ((byteToCheckAddress >> 8) << 8);
        }
    }
}


bool isFlightLogged(u8_t flightNumbToCheck) {



    u8_t addressToCheck[3] = {0x00, 0x00, 0x00};

    u16_t flightAddressLoc = FLIGHTZEROSTART + (flightNumbToCheck * 0x03);

    AT25SEreadSample(flightAddressLoc, 0x03, addressToCheck);

    u32_t possibleStart = addressToCheck[0] << 0 | addressToCheck[1] << 8 | addressToCheck[2] << 16;

    if (possibleStart == 0xFFFFFF) {
        return false;
    } else {
        return true;
    }


}