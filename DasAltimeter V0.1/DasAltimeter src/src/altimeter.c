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


    for (u8_t flightNumbToCheck = 0; flightNumbToCheck < 11; ++flightNumbToCheck) {

        if (!isFlightLogged(flightNumbToCheck)) {

            my_altimeter->myFlashMemory.currentAddress = findNextBegining(flightNumbToCheck);
            my_altimeter->flightNumb = flightNumbToCheck;

            beep(500);
            delay_ms(500);
            while (flightNumbToCheck > 0) {
                beep(500);
                delay_ms(500);
                --flightNumbToCheck;
            }
            delay_ms(500);
            break;
        }
    }
}

u32_t findNextBegining(u8_t lastFlightStart) {

    u32_t flightToSearch = (lastFlightStart * 0x03) + FLIGHTZEROSTART;

    u8_t searchStartByte[3] = {0x00, 0x00, 0x00};
    AT25SEreadSample(flightToSearch, 0x03, searchStartByte);

    u32_t searchStart = searchStartByte[0] << 0 | searchStartByte[1] << 8 | searchStartByte[2] << 16;

    u32_t byteToCheckAddress = searchStart;

    bool flightEndFound = false;
    while (!flightEndFound) {

        u8_t byteToCheck = AT25SFGetByte(byteToCheckAddress);

        if (byteToCheck == 0x41) {
            byteToCheckAddress += 24;
        } else if (byteToCheck == 0x46) {
            byteToCheckAddress += 17;
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
