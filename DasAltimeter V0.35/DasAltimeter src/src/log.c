#include <util.h>
#include <flight.h>
#include <boardDefines.h>
#include <math.h>

uint32_t currentAddress;

uint32_t endingAddress;

u8_t pageToWrite[256];

u8_t pageLocation;

bool pageReady;

u8_t padSensorBuffer[10][SENSOR_LOG_LENGTH];

u8_t padBufferLocation = 0;

u16_t flightAddressLocationInIndex;

u8_t flightStartAddress[3];


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

void writePadBuffer(u8_t bytesToWrite, u8_t* data) {
    u8_t i, j;

    for (i = 1; i < 10; i++) {
        for (j = 0; j < bytesToWrite; j++) {
            padSensorBuffer[i-1][j] = padSensorBuffer[i][j];
        }
    }

    for (i = 0; i < bytesToWrite ; i++) {
        padSensorBuffer[9][i] = data[i];
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

    writeLog = false;

    u8_t bytesToSend = SENSOR_LOG_LENGTH;
    u8_t dataToSend[SENSOR_LOG_LENGTH];

    float fractionalAccel = accel - (int16_t)(accel);
    int16_t fractAccelPart = fractionalAccel * 1000;

    float fractionalVelocity = velocity - (int16_t)(velocity);
    int16_t fractVelocityPart = fractionalVelocity * 1000;

    dataToSend[0] = SENSOR_LOG;

    dataToSend[1] = sample.sampleTick >> 0;
    dataToSend[2] = sample.sampleTick >> 8;
    dataToSend[3] = sample.sampleTick >> 16;
    dataToSend[4] = sample.sampleTick >> 24;

    dataToSend[5] = altitudeAGL() >> 0;
    dataToSend[6] = altitudeAGL() >> 8;
    dataToSend[7] = altitudeAGL() >> 16;
    dataToSend[8] = altitudeAGL() >> 24;



    dataToSend[9] = (int16_t)(accel) >> 0;
    dataToSend[10] = (int16_t)(accel) >> 8;
    dataToSend[11] = fractAccelPart >> 0;
    dataToSend[12] = fractAccelPart >> 8;


    dataToSend[13] = (int16_t)(velocity) >> 0;
    dataToSend[14] = (int16_t)(velocity) >> 8;
    dataToSend[15] = fractVelocityPart >> 0;
    dataToSend[16] = fractVelocityPart >> 8;

    dataToSend[17] = sample.accelXint >> 0;
    dataToSend[18] = sample.accelXint >> 8;

    dataToSend[19] = sample.accelYint >> 0;
    dataToSend[20] = sample.accelYint >> 8;

    dataToSend[21] = sample.gyroXint >> 0;
    dataToSend[22] = sample.gyroXint >> 8;

    dataToSend[23] = sample.gyroYint >> 0;
    dataToSend[24] = sample.gyroYint >> 8;

    dataToSend[25] = sample.gyroZint >> 0;
    dataToSend[26] = sample.gyroZint >> 8;

    dataToSend[27] = sample.analogRaw >> 0;
    dataToSend[28] = sample.analogRaw >> 8;


    if (flightState == flightPad) {
        writePadBuffer(bytesToSend, dataToSend);

    } else {

        makePage(bytesToSend, dataToSend);

        if (pageReady) {
            pageReady = false;
            pinToggle(LedPin);
            u8_t bytesWritten = AT25SEWritePage(currentAddress,pageToWrite);
            currentAddress = (currentAddress + 0x100);
        }
    }
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

void writeFlightStartAddress() {
    AT25SFWriteBytes(flightAddressLocationInIndex,3,flightStartAddress);
}


void writeFlightEndAddress() {

    u32_t endingAddress = currentAddress;

    u8_t address[3] = {endingAddress >> 0, endingAddress >> 8, endingAddress >> 16};

    u16_t flightAddressLoc = FLIGHTZEROEND + (flightNumb * 0x06);
    AT25SFWriteBytes(flightAddressLoc, 3, address);
    delay_ms(10);
}

void writeGroundLog() {
    u8_t i;

    for (i = 0; i < 10; i++) {
        makePage(SENSOR_LOG_LENGTH, &padSensorBuffer[i][0]);

        if (pageReady) {
            pageReady = false;
            pinToggle(LedPin);
            u8_t bytesWritten = AT25SEWritePage(currentAddress,pageToWrite);
            currentAddress = (currentAddress + 0x100);
        }

    }

}


void findNewFlightStart() {

    for (u8_t flightNumbToCheck = 0; flightNumbToCheck < 20; ++flightNumbToCheck) {

        if (!isFlightLogged(flightNumbToCheck)) {

            if (flightNumbToCheck > 0) {

                u32_t startAddress = FindFlightEndingAddress(flightNumbToCheck - 1);
                delay_ms(5);

                currentAddress = startAddress;

                flightStartAddress[0] = startAddress >> 0;
                flightStartAddress[1] = startAddress >> 8;
                flightStartAddress[2] = startAddress >> 16;

                flightAddressLocationInIndex = FLIGHTZEROSTART + (flightNumbToCheck * 0x06);

                // for flight 0, starting address is always 0x00100 (beginning of second 4k block)
            } else {
                flightStartAddress[0] = 0x00;
                flightStartAddress[1] = 0x10;
                flightStartAddress[2] = 0x00;

                flightAddressLocationInIndex = FLIGHTZEROSTART;

                currentAddress = 0x001000;
                beep(1000);
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
