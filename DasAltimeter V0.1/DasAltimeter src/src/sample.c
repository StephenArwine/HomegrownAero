#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>



void sampleTick(Altimeter *my_altimeter) {

    my_altimeter->batV = (0.001906 * adc_read(senseBatPin));



    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;


    pinLow(cs_baro);
    // dummy_rx = spiDataTransfer(SPI2, 0x50);
    byteOut(spi2SCK,spi2MOSI, 0x50);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawTempatureData = readMS5803AdcResults();

    pinLow(cs_baro);
    //  dummy_rx = spiDataTransfer(SPI2, 0x42);
    byteOut(spi2SCK,spi2MOSI, 0x42);
    pinHigh(cs_baro);
    delay_ms(2);
    my_altimeter->myBarometer.rawPressureData = readMS5803AdcResults();


    ConvertPressureTemperature(&my_altimeter->myBarometer);
    pascalToCent(&my_altimeter->myBarometer);
    my_altimeter->myBarometer.heightFeet = 0.03281 * my_altimeter->myBarometer.heightCm;





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
