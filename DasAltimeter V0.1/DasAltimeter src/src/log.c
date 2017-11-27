#include <util.h>
#include <log.h>
#include <flight.h>
#include <boardDefines.h>




void logSensors(Altimeter *my_altimeter) {

    u8_t bytesToSend = 23;
    u8_t dataToSend[23];


    dataToSend[0] = SENSOR_LOG;
    dataToSend[1] = my_altimeter->sampleTick >> 0;
    dataToSend[2] = my_altimeter->sampleTick >> 8;
    dataToSend[3] = my_altimeter->sampleTick >> 16;
    dataToSend[4] = my_altimeter->sampleTick >> 24;

    dataToSend[5] = my_altimeter->myBarometer.heightCm >> 0;
    dataToSend[6] = my_altimeter->myBarometer.heightCm >> 8;
    dataToSend[7] = my_altimeter->myBarometer.heightCm >> 16;
    dataToSend[8] = my_altimeter->myBarometer.heightCm >> 24;

    dataToSend[9] = my_altimeter->myIMU.accelXRaw >> 0;
    dataToSend[10] = my_altimeter->myIMU.accelXRaw >> 8;
    dataToSend[11] = my_altimeter->myIMU.accelYRaw >> 0;
    dataToSend[12] = my_altimeter->myIMU.accelYRaw >> 8;
    dataToSend[13] = my_altimeter->myIMU.accelZRaw >> 0;
    dataToSend[14] = my_altimeter->myIMU.accelZRaw >> 8;

    dataToSend[15] = my_altimeter->myIMU.gyroXRaw >> 0;
    dataToSend[16] = my_altimeter->myIMU.gyroXRaw >> 8;
    dataToSend[17] = my_altimeter->myIMU.gyroYRaw >> 0;
    dataToSend[18] = my_altimeter->myIMU.gyroYRaw >> 8;
    dataToSend[19] = my_altimeter->myIMU.gyroZRaw >> 0;
    dataToSend[20] = my_altimeter->myIMU.gyroZRaw >> 8;

    dataToSend[21] = my_altimeter->myAnalogAccelerometer.analogRaw >> 0;
    dataToSend[22] = my_altimeter->myAnalogAccelerometer.analogRaw >> 8;



    for (u8_t dataByte = 0; dataByte < bytesToSend; ++dataByte) {
        usartDataOut(USART3, dataToSend[dataByte]);
    }
     //u8_t bytesSent = AT25SEWritePage(my_altimeter->currentAddress,bytesToSend,dataToSend);

}



