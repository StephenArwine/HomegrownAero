#include <util.h>
#include <flight.h>
#include <boardDefines.h>
#include <math.h>

uint32_t currentAddress;

uint32_t endingAddress;

u8_t pageToWrite[256];

u8_t pageLocation;

bool pageReady;


void makePage(u8_t bytesToWrite, u8_t* data) {


    u8_t location = pageLocation;
    u8_t bytesWritten = 0;

    if ((location + bytesToWrite) > 0xFF) {
        for (u16_t dataByte = 0; (dataByte + location) <= 0xFF; ++dataByte) {
            pageBuffer[(dataByte + location)] = data[dataByte];
            bytesWritten++;
        }
        pageLocation = 0x00;
        for(u16_t i = 0; i <= 0xFF; ++i) {
            pageToWrite[i] = pageBuffer[i];
            pageBuffer[i] = 0;
        }
        for (u16_t dataByte = 0; bytesWritten < bytesToWrite; ++dataByte) {
            pageBuffer[dataByte] = data[bytesWritten];
            bytesWritten++;
            pageLocation = dataByte + 1;
        }
        pageReady = true;

    } else {
        for (u16_t dataByte = 0; dataByte <= bytesToWrite; ++dataByte) {
            pageBuffer[(dataByte + location)] = data[dataByte];
            bytesWritten++;
            pageLocation = location + dataByte;
        }
    }
}


void logFlight() {

    u8_t bytesToSend = 16;
    u8_t dataToSend[16];

    dataToSend[0] = FLIGHT_LOG;
    dataToSend[1] = 0;

    dataToSend[2] = offsets.offsetBufferTime >> 0;
    dataToSend[3] = offsets.offsetBufferTime >> 8;
    dataToSend[4] = offsets.offsetBufferTime >> 16;
    dataToSend[5] = offsets.offsetBufferTime >> 24;

    dataToSend[6] = offsets.groundOffset >> 0;
    dataToSend[7] = offsets.groundOffset >> 8;
    dataToSend[8] = offsets.groundOffset >> 16;
    dataToSend[9] = offsets.groundOffset >> 24;

    dataToSend[10] = offsets.groundTemperature >> 0;
    dataToSend[11] = offsets.groundTemperature >> 8;
    dataToSend[12] = offsets.groundTemperature >> 16;
    dataToSend[13] = offsets.groundTemperature >> 24;

    //dataToSend[14] = accel >> 0;
    dataToSend[15] = 0xfe;

    makePage(bytesToSend, dataToSend);
}

void logEvent(u8_t eventType) {

    u8_t bytesToSend = 12;
    u8_t dataToSend[12];

    dataToSend[0] = EVENT_LOG;
    dataToSend[1] = eventType;
    dataToSend[2] = sample.sampleTick >> 0;
    dataToSend[3] = sample.sampleTick >> 8;
    dataToSend[4] = sample.sampleTick >> 16;
    dataToSend[5] = sample.sampleTick >> 24;

    dataToSend[6] = altitudeAGL() >> 0;
    dataToSend[7] = altitudeAGL() >> 8;
    dataToSend[8] = altitudeAGL() >> 16;
    dataToSend[9] = altitudeAGL() >> 24;

    dataToSend[10] = sample.voltage.batV >> 0;
    dataToSend[11] = sample.voltage.batV >> 8;

    makePage(bytesToSend, dataToSend);

}

void logSensors() {

    u8_t bytesToSend = 24;
    u8_t dataToSend[24];

    dataToSend[0] = SENSOR_LOG;

    dataToSend[1] = sample.sampleTick >> 0;
    dataToSend[2] = sample.sampleTick >> 8;
    dataToSend[3] = sample.sampleTick >> 16;
    dataToSend[4] = sample.sampleTick >> 24;

    dataToSend[5] = altitudeAGL() >> 0;
    dataToSend[6] = altitudeAGL() >> 8;
    dataToSend[7] = altitudeAGL() >> 16;
    dataToSend[8] = altitudeAGL() >> 24;

    volatile float fractionalAccel = accel - (int16_t)(accel);
    volatile int16_t fractAccelPart = fractionalAccel * 1000;

    dataToSend[9] = (int16_t)(accel) >> 0;
    dataToSend[10] = (int16_t)(accel) >> 8;
    dataToSend[11] = fractAccelPart >> 0;
    dataToSend[12] = fractAccelPart >> 8;


     float fractionalVelocity = velocity - (int16_t)(velocity);
     int16_t fractVelocityPart = fractionalVelocity * 1000;

    dataToSend[13] = (int16_t)(velocity) >> 0;
    dataToSend[14] = (int16_t)(velocity) >> 8;
    dataToSend[15] = fractVelocityPart >> 0;
    dataToSend[16] = fractVelocityPart >> 8;

//     if ( ((fractVelocityPart < 0) & ((int16_t)(velocity) > 0)) | ((fractVelocityPart > 0) & ((int16_t)(velocity) < 0)) ) {
//         beep(200);
//     }

    dataToSend[17] = (u32_t)sample.altitudefeet >> 0;
    dataToSend[18] = (u32_t)sample.altitudefeet >> 8;
    dataToSend[19] = (u32_t)sample.altitudefeet >> 16;
    dataToSend[20] = (u32_t)sample.altitudefeet >> 24;

    float fractionalAccelraw = sample.accelZ - (int16_t)(sample.accelZ);
    u8_t fractAccelRawPart = fractionalAccelraw;

    dataToSend[21] = (int16_t)(sample.accelZ) >> 0;
    dataToSend[22] = (int16_t)(sample.accelZ) >> 8;
    dataToSend[23] = fractAccelRawPart;

    //dataToSend[9] = my_altimeter->myIMU.accelXRaw >> 0;
    //dataToSend[10] = my_altimeter->myIMU.accelXRaw >> 8;
    //dataToSend[11] = my_altimeter->myIMU.accelYRaw >> 0;
    //dataToSend[12] = my_altimeter->myIMU.accelYRaw >> 8;
    //dataToSend[21] = sample.accelZ >> 0;
    //dataToSend[22] = sample.accelZ >> 8;
    //dataToSend[17] = my_altimeter->myIMU.gyroYRaw >> 0;
    //dataToSend[18] = my_altimeter->myIMU.gyroYRaw >> 8;
    //dataToSend[19] = my_altimeter->myIMU.gyroZRaw >> 0;
    //dataToSend[20] = my_altimeter->myIMU.gyroZRaw >> 8;


    //dataToSend[21] = my_altimeter->myAnalogAccelerometer.analogRaw >> 0;
    //dataToSend[22] = my_altimeter->myAnalogAccelerometer.analogRaw >> 8;

    /*
     dataToSend[1] = 0xff;
     dataToSend[2] = 0xff;
     dataToSend[3] = 0xff;
     dataToSend[4] = 0xff;
     dataToSend[5] = 0xff;
     dataToSend[6] = 0xff;
     dataToSend[7] = 0xff;
     dataToSend[8] = 0xff;
     dataToSend[9] = 0xff;
     dataToSend[10] = 0xff;
     dataToSend[11] = 0xff;
     dataToSend[12] = 0xff;
     dataToSend[13] = 0xff;
     dataToSend[14] = 0xff;
     dataToSend[15] = 0xff;
     dataToSend[16] = 0xff;
     dataToSend[17] = 0xff;
     dataToSend[18] = 0xff;
     dataToSend[19] = 0xff;
     dataToSend[20] = 0xff;
     dataToSend[21] = 0xfc;
     dataToSend[22] = 0xfd;
     dataToSend[23] = 0xfe;

     */

    makePage(bytesToSend, dataToSend);
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



void writeFlightEndAddress() {

    u32_t endingAddress = currentAddress;

    u8_t address[3] = {endingAddress >> 0, endingAddress >> 8, endingAddress >> 16};

    u16_t flightAddressLoc = FLIGHTZEROEND + (flightNumb * 0x06);
    AT25SFWriteBytes(flightAddressLoc, 3, address);
    delay_ms(10);

}




void findNewFlightStart() {

    for (u8_t flightNumbToCheck = 0; flightNumbToCheck < 20; ++flightNumbToCheck) {

        if (!isFlightLogged(flightNumbToCheck)) {

            if (flightNumbToCheck > 0) {


                u32_t startAddress = FindFlightEndingAddress(flightNumbToCheck - 1);
                delay_ms(5);

                currentAddress = startAddress;

                u8_t address[3] = {startAddress >> 0, startAddress >> 8, startAddress >> 16};

                u16_t flightAddressLoc = FLIGHTZEROSTART + (flightNumbToCheck * 0x06);
                AT25SFWriteBytes(flightAddressLoc, 3, address);

                // for flight 0, starting address is always 0x00100 (beginning of second 4k block)
            } else {
                u8_t address[3] = {0x00,0x10,0x00};
                AT25SFWriteBytes(FLIGHTZEROSTART, 3, address);
                currentAddress = 0x001000;
                beep(1500);
            }
            flightNumb = flightNumbToCheck;

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
