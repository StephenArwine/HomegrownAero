#include <util.h>
#include <log.h>
#include <flight.h>
#include <boardDefines.h>
#include <math.h>


void makePage(Altimeter *my_altimeter, u8_t bytesToWrite, u8_t *data) {


    u8_t location = my_altimeter->myFlashMemory.pageLocation;
    u8_t bytesWritten = 0;

    if ((location + bytesToWrite) >= 0xFF) {
        for (u16_t dataByte = 0; (dataByte + location) < 0xFF; ++dataByte) {
            my_altimeter->myFlashMemory.pageBuffer[(dataByte + location)] = data[dataByte];
            bytesWritten++;
        }
        my_altimeter->myFlashMemory.pageLocation = 0x00;
        for(u16_t i = 0; i <= 0xFF; ++i) {
            my_altimeter->myFlashMemory.pageToWrite[i] = my_altimeter->myFlashMemory.pageBuffer[i];
            my_altimeter->myFlashMemory.pageBuffer[i] = 0;
        }
        for (u8_t dataByte = 0; bytesWritten < bytesToWrite; ++dataByte) {
            my_altimeter->myFlashMemory.pageBuffer[dataByte] = data[bytesWritten];
            bytesWritten++;
            my_altimeter->myFlashMemory.pageLocation = dataByte + 1;
        }

        my_altimeter->myFlashMemory.pageReady = true;

    } else {
        for (u16_t dataByte = 0; dataByte <= bytesToWrite; ++dataByte) {
            my_altimeter->myFlashMemory.pageBuffer[(dataByte + location)] = data[dataByte];
            bytesWritten++;
            my_altimeter->myFlashMemory.pageLocation = location + dataByte + 1;
        }

    }
}


void logFlight(Altimeter *my_altimeter) {

    u8_t bytesToSend = 16;
    u8_t dataToSend[16];

    dataToSend[0] = FLIGHT_LOG;
    dataToSend[1] = my_altimeter->flightNumb;

    dataToSend[2] = my_altimeter->myIMU.offsetBufferTime >> 0;
    dataToSend[3] = my_altimeter->myIMU.offsetBufferTime >> 8;
    dataToSend[4] = my_altimeter->myIMU.offsetBufferTime >> 16;
    dataToSend[5] = my_altimeter->myIMU.offsetBufferTime >> 24;

    dataToSend[6] = my_altimeter->myBarometer.groundOffset >> 0;
    dataToSend[7] = my_altimeter->myBarometer.groundOffset >> 8;
    dataToSend[8] = my_altimeter->myBarometer.groundOffset >> 16;
    dataToSend[9] = my_altimeter->myBarometer.groundOffset >> 24;

    dataToSend[10] = my_altimeter->myBarometer.groundTemperature >> 0;
    dataToSend[11] = my_altimeter->myBarometer.groundTemperature >> 8;
    dataToSend[12] = my_altimeter->myBarometer.groundTemperature >> 16;
    dataToSend[13] = my_altimeter->myBarometer.groundTemperature >> 24;

    dataToSend[14] = my_altimeter->myIMU.accelXRaw >> 0;
    dataToSend[15] = my_altimeter->myIMU.accelXRaw >> 8;

    makePage(my_altimeter, bytesToSend, dataToSend);
}

void logEvent(Altimeter *my_altimeter, u8_t eventType) {

    u8_t bytesToSend = 12;
    u8_t dataToSend[12];

    dataToSend[0] = EVENT_LOG;
	dataToSend[1] = eventType;
    dataToSend[2] = my_altimeter->sampleTick >> 0;
    dataToSend[3] = my_altimeter->sampleTick >> 8;
    dataToSend[4] = my_altimeter->sampleTick >> 16;
    dataToSend[5] = my_altimeter->sampleTick >> 24;

    dataToSend[6] = my_altimeter->Altitude >> 0;
    dataToSend[7] = my_altimeter->Altitude >> 8;
    dataToSend[8] = my_altimeter->Altitude >> 16;
    dataToSend[9] = my_altimeter->Altitude >> 24;

    dataToSend[10] = my_altimeter->myVoltages.batV >> 0;
    dataToSend[11] = my_altimeter->myVoltages.batV >> 8;

    makePage(my_altimeter, bytesToSend, dataToSend);

}

void logSensors(Altimeter *my_altimeter) {

    u8_t bytesToSend = 23;
    u8_t dataToSend[23];

    dataToSend[0] = SENSOR_LOG;
    dataToSend[1] = my_altimeter->sampleTick >> 0;
    dataToSend[2] = my_altimeter->sampleTick >> 8;
    dataToSend[3] = my_altimeter->sampleTick >> 16;
    dataToSend[4] = my_altimeter->sampleTick >> 24;

    dataToSend[5] = my_altimeter->Altitude >> 0;
    dataToSend[6] = my_altimeter->Altitude >> 8;
    dataToSend[7] = my_altimeter->Altitude >> 16;
    dataToSend[8] = my_altimeter->Altitude >> 24;

    float fractionalAccel = my_altimeter->Acceleration - (int16_t)(my_altimeter->Acceleration);
    u16_t fractAccelPart = fractionalAccel * 1000;

    dataToSend[9] = (int16_t)(my_altimeter->Acceleration) >> 0;
    dataToSend[10] = (int16_t)(my_altimeter->Acceleration) >> 8;
    dataToSend[11] = fractAccelPart >> 0;
    dataToSend[12] = fractAccelPart >> 8;


    float fractionalVelocity = my_altimeter->Velocity - (int16_t)(my_altimeter->Velocity);
    u16_t fractVelocityPart = fractionalVelocity * 1000;

    dataToSend[13] = (int16_t)(my_altimeter->Velocity) >> 0;
    dataToSend[14] = (int16_t)(my_altimeter->Velocity) >> 8;
    dataToSend[15] = fractVelocityPart >> 0;
    dataToSend[16] = fractVelocityPart >> 8;

    dataToSend[17] = my_altimeter->myBarometer.heightFeet >> 0;
    dataToSend[18] = my_altimeter->myBarometer.heightFeet >> 8;
    dataToSend[19] = my_altimeter->myBarometer.heightFeet >> 16;
    dataToSend[20] = my_altimeter->myBarometer.heightFeet >> 24;



    //dataToSend[9] = my_altimeter->myIMU.accelXRaw >> 0;
    //dataToSend[10] = my_altimeter->myIMU.accelXRaw >> 8;
    //dataToSend[11] = my_altimeter->myIMU.accelYRaw >> 0;
    //dataToSend[12] = my_altimeter->myIMU.accelYRaw >> 8;
    //dataToSend[13] = my_altimeter->myIMU.accelZRaw >> 0;
    //dataToSend[14] = my_altimeter->myIMU.accelZRaw >> 8;
    //dataToSend[17] = my_altimeter->myIMU.gyroYRaw >> 0;
    //dataToSend[18] = my_altimeter->myIMU.gyroYRaw >> 8;
    //dataToSend[19] = my_altimeter->myIMU.gyroZRaw >> 0;
    //dataToSend[20] = my_altimeter->myIMU.gyroZRaw >> 8;


    dataToSend[21] = my_altimeter->myAnalogAccelerometer.analogRaw >> 0;
    dataToSend[22] = my_altimeter->myAnalogAccelerometer.analogRaw >> 8;

    makePage(my_altimeter, bytesToSend, dataToSend);
}



u32_t getFlightStartAddress(u8_t flightToFind) {

    u8_t addressToCheck[3];

    u16_t flightAddressLoc = FLIGHTZEROSTART + (flightToFind * 0x06);

    AT25SEreadSample(flightAddressLoc, 0x03, addressToCheck);

    u32_t flightStartAddress = addressToCheck[0] << 0 | addressToCheck[1] << 8 | addressToCheck[2] << 16;

    return flightStartAddress;
}

u32_t FindFlightEndingAddress(u8_t findThisFlightsEnd) {

    u8_t addressToCheck[3];

    u16_t flightAddressLoc = FLIGHTZEROEND + (findThisFlightsEnd * 0x06);

    AT25SEreadSample(flightAddressLoc, 0x03, addressToCheck);

    u32_t flightEndAddress = addressToCheck[0] << 0 | addressToCheck[1] << 8 | addressToCheck[2] << 16;

    return flightEndAddress;
}

bool isFlightLogged(u8_t flightNumbToCheck) {

    u32_t possibleStart = getFlightStartAddress(flightNumbToCheck);

    if (possibleStart == 0xFFFFFF) {
        return false;
    } else {
        return true;
    }
}



void writeFlightEndAddress(Altimeter *my_altimeter) {

    u32_t endingAddress = my_altimeter->myFlashMemory.currentAddress;

    u8_t address[3] = {endingAddress >> 0, endingAddress >> 8, endingAddress >> 16};

    u16_t flightAddressLoc = FLIGHTZEROEND + (my_altimeter->flightNumb * 0x06);
    AT25SFWriteBytes(flightAddressLoc, 3, address);
    delay_ms(10);

}




void findNewFlightStart(Altimeter *my_altimeter) {

    for (u8_t flightNumbToCheck = 0; flightNumbToCheck < 20; ++flightNumbToCheck) {

        if (!isFlightLogged(flightNumbToCheck)) {

            if (flightNumbToCheck > 0) {


                u32_t startAddress = FindFlightEndingAddress(flightNumbToCheck - 1);
                delay_ms(5);

                my_altimeter->myFlashMemory.currentAddress = startAddress;

                u8_t address[3] = {startAddress >> 0, startAddress >> 8, startAddress >> 16};

                u16_t flightAddressLoc = FLIGHTZEROSTART + (flightNumbToCheck * 0x06);
                AT25SFWriteBytes(flightAddressLoc, 3, address);

                // for flight 0, starting address is always 0x00100 (beginning of second 4k block)
            } else {
                u8_t address[3] = {0x00,0x10,0x00};
                AT25SFWriteBytes(FLIGHTZEROSTART, 3, address);
                my_altimeter->myFlashMemory.currentAddress = 0x001000;
                beep(1500);
            }
            my_altimeter->flightNumb = flightNumbToCheck;

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
