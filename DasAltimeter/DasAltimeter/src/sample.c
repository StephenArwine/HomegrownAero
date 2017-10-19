#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>



void sampleTick(Altimeter *my_altimeter) {
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
         dummy_rx = spiDataTransfer(SPI1,ACCEL_CONFIG | ACCEL_XOUT_H); 
         my_altimeter->DofIMU.accelXHigh  = spiDataTransfer(SPI1,dummy_Tx); 
         my_altimeter->DofIMU.accelXLow  = spiDataTransfer(SPI1,dummy_Tx); 
        pinHigh(cs_imu); 

        //check if last bit in second byte is 1 therefor negative number 
        int16_t negativex = (my_altimeter->DofIMU.accelXLow & (1 <<7)) != 0; 
        if (negativex) { 
             // if negative then preform 2's complement to int conversion 
            my_altimeter->DofIMU.accelX = ((my_altimeter->DofIMU.accelXLow | ~((1 << 8) - 1)) << 8 ) | my_altimeter->DofIMU.accelXLow; 
         } else { 
             my_altimeter->DofIMU.accelX = (my_altimeter->DofIMU.accelXLow << 8) + my_altimeter->DofIMU.accelXLow; 
         } 
         my_altimeter->DofIMU.accelX = my_altimeter->DofIMU.accelX *0.00006103; 



         pinLow(cs_imu); 
         dummy_rx = spiDataTransfer(SPI1,ACCEL_CONFIG | ACCEL_YOUT_H); 
         my_altimeter->DofIMU.accelYHigh = spiDataTransfer(SPI1,dummy_Tx); 
         my_altimeter->DofIMU.accelYLow = spiDataTransfer(SPI1,dummy_Tx); 
        pinHigh(cs_imu); 

        //check if last bit in second byte is 1 therefor negative number 
        int16_t negativey = (my_altimeter->DofIMU.accelYLow & (1 <<7)) != 0; 
        if (negativey) { 
             // if negative then preform 2's complement to int conversion 
            my_altimeter->DofIMU.accelY = ((my_altimeter->DofIMU.accelYLow | ~((1 << 8) - 1)) << 8 ) | my_altimeter->DofIMU.accelYLow; 
         } else { 
             my_altimeter->DofIMU.accelY = (my_altimeter->DofIMU.accelYLow << 8) + my_altimeter->DofIMU.accelYLow; 
         } 
         my_altimeter->DofIMU.accelY = my_altimeter->DofIMU.accelY *0.00006103; 


         pinLow(cs_imu); 
         dummy_rx = spiDataTransfer(SPI1,ACCEL_CONFIG | ACCEL_ZOUT_H); 
         my_altimeter->DofIMU.accelZHigh = spiDataTransfer(SPI1,dummy_Tx); 
         my_altimeter->DofIMU.accelZLow = spiDataTransfer(SPI1,dummy_Tx); 
        pinHigh(cs_imu);
 
        //check if last bit in second byte is 1 therefor negative number 
        int16_t negativez = (my_altimeter->DofIMU.accelZLow & (1 <<7)) != 0; 
        if (negativez) { 
             // if negative then preform 2's complement to int conversion 
            my_altimeter->DofIMU.accelZ = ((my_altimeter->DofIMU.accelZLow | ~((1 << 8) - 1)) << 8 ) | my_altimeter->DofIMU.accelZLow; 
         } else { 
             my_altimeter->DofIMU.accelZ = (my_altimeter->DofIMU.accelZLow << 8) + my_altimeter->DofIMU.accelZLow; 
         } 
         my_altimeter->DofIMU.accelZ = my_altimeter->DofIMU.accelZ *0.00006103; 






}
