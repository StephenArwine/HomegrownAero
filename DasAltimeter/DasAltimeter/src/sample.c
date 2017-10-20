#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>



void sampleTick(Altimeter *my_altimeter) {

    my_altimeter->batV = (0.0020676 * adc_read(senseBat));



    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_baro);
    dummy_rx = spiDataTransfer(SPI1, 0x50);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawTempatureData = readMS5803AdcResults();

    pinLow(cs_baro);
    dummy_rx = spiDataTransfer(SPI1, 0x42);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawPressureData = readMS5803AdcResults();
    flight();

    ConvertPressureTemperature(&my_altimeter->myBarometer);
    pascalToCent(&my_altimeter->myBarometer);
    my_altimeter->myBarometer.heightFeet = 0.03281 * my_altimeter->myBarometer.heightCm;

    uint8_t rData;
    uint8_t rData2;

    pinLow(cs_imu);
    dummy_rx = spiDataTransfer(SPI1, ACCEL_XOUT_H);
    my_altimeter->myIMU.accelXHigh  = spiDataTransfer(SPI1,dummy_Tx);
    my_altimeter->myIMU.accelXLow  = spiDataTransfer(SPI1,dummy_Tx);
    pinHigh(cs_imu);

    //check if last bit in second byte is 1 therefor negative number
    int16_t negativex = (my_altimeter->myIMU.accelXLow & (1 <<7)) != 0;
    if (negativex) {
        // if negative then preform 2's complement to int conversion
        my_altimeter->myIMU.accelX = ((my_altimeter->myIMU.accelXLow | ~((1 << 8) - 1)) << 8 ) | my_altimeter->myIMU.accelXHigh;
    } else {
        my_altimeter->myIMU.accelX = (my_altimeter->myIMU.accelXLow << 8) + my_altimeter->myIMU.accelXHigh;
    }
    my_altimeter->myIMU.accelX = my_altimeter->myIMU.accelX *0.00006103;



    pinLow(cs_imu);
    dummy_rx = spiDataTransfer(SPI1, ACCEL_YOUT_H);
    my_altimeter->myIMU.accelYHigh = spiDataTransfer(SPI1,dummy_Tx);
    my_altimeter->myIMU.accelYLow = spiDataTransfer(SPI1,dummy_Tx);
    pinHigh(cs_imu);

    //check if last bit in second byte is 1 therefor negative number
    int16_t negativey = (my_altimeter->myIMU.accelYLow & (1 <<7)) != 0;
    if (negativey) {
        // if negative then preform 2's complement to int conversion
        my_altimeter->myIMU.accelY = ((my_altimeter->myIMU.accelYLow | ~((1 << 8) - 1)) << 8 ) | my_altimeter->myIMU.accelYHigh;
    } else {
        my_altimeter->myIMU.accelY = (my_altimeter->myIMU.accelYLow << 8) + my_altimeter->myIMU.accelYHigh;
    }
    my_altimeter->myIMU.accelY = my_altimeter->myIMU.accelY *0.00006103;


    pinLow(cs_imu);
    dummy_rx = spiDataTransfer(SPI1, ACCEL_ZOUT_H);
    my_altimeter->myIMU.accelZHigh = spiDataTransfer(SPI1,dummy_Tx);
    my_altimeter->myIMU.accelZLow = spiDataTransfer(SPI1,dummy_Tx);
    pinHigh(cs_imu);

    //check if last bit in second byte is 1 therefor negative number
    int16_t negativez = (my_altimeter->myIMU.accelZLow & (1 <<7)) != 0;
    if (negativez) {
        // if negative then preform 2's complement to int conversion
        my_altimeter->myIMU.accelZ = ((my_altimeter->myIMU.accelZLow | ~((1 << 8) - 1)) << 8 ) | my_altimeter->myIMU.accelZHigh;
    } else {
        my_altimeter->myIMU.accelZ = (my_altimeter->myIMU.accelZLow << 8) + my_altimeter->myIMU.accelZHigh;
    }
    my_altimeter->myIMU.accelZ = my_altimeter->myIMU.accelZ *0.00006103;






}
