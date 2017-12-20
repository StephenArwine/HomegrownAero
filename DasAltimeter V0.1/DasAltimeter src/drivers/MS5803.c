
#include <MS5803.h>
#include <boardDefines.h>
#include <math.h>
//#include <tgmath.h>


u32_t readMS5803AdcResults() {

    u8_t dummy_tx = 0xFF;
    u8_t dummy_rx;

    pinLow(cs_baro);
//    dummy_rx = spiDataTransfer(SPI1, cmdAdcRead_);
//    u8_t _byte1 = spiDataTransfer(SPI1,dummy_tx);
//    u8_t _byte2 = spiDataTransfer(SPI1,dummy_tx);
//    u8_t _byte3 = spiDataTransfer(SPI1,dummy_tx);
    byteOut(spi2SCK,spi2MOSI, cmdAdcRead_);
    u8_t _byte1 = byteIn(spi2SCK,spi2MISO);
    u8_t _byte2 = byteIn(spi2SCK,spi2MISO);
    u8_t _byte3 = byteIn(spi2SCK,spi2MISO);
    pinHigh(cs_baro);

    u32_t _receive = (_byte1 << 16) | (_byte2 << 8) | (_byte3);
    return _receive;
}

void ConvertPressureTemperature(u32_t pressureRaw, u32_t tempRaw, u32_t* temperatureCelcus, int32_t* pressureMbar ) {
    const u16_t C1 = coefficients_[0];
    const u16_t C2 = coefficients_[1];
    const u16_t C3 = coefficients_[2];
    const u16_t C4 = coefficients_[3];
    const u16_t C5 = coefficients_[4];
    const u16_t C6 = coefficients_[5];

    // calculate 1st order pressure and temperature (MS5607 1st order algorithm)
    const  int32_t dT = (tempRaw) - ((int32_t)C5 << 8);
    *temperatureCelcus  = 2000 + (((int64_t)dT * C6) >> 23) ;

    const  int64_t OFF   = ((int64_t)C2 << 16) + ((C4 * (int64_t)dT) >> 7);
     const  int64_t SENS  =  ((int64_t)C1 << 15) + ((C3 * (int64_t)dT) >> 8);
    *pressureMbar = ((((pressureRaw * SENS) >> 21) - OFF) >> 15);

}

double paToFeetNOAA(int32_t pressureMbar) {

    double lower =(((double)(pressureMbar)/10)/1013.25);
    double exponent = 0.190284;

    double altFeet = (1-pow(lower,exponent))*145366.45;
    return altFeet;

}

/*
void pascalToCent(Barometer *my_barometer) {

    static const int32_t PZLUT_ENTRIES = 77;
    static const int32_t PA_INIT       = 104908;
    static const int32_t PA_DELTA      = 1024;

    static const int32_t lookupTable[] = {
        -29408, -21087, -12700,  -4244,   4279,
        12874,  21541,  30281,  39095,  47986,
        56953,  66000,  75126,  84335,  93628,
        103006, 112472, 122026, 131672, 141410,
        151244, 161174, 171204, 181335, 191570,
        201911, 212361, 222922, 233597, 244388,
        255300, 266334, 277494, 288782, 300204,
        311761, 323457, 335297, 347285, 359424,
        371719, 384174, 396795, 409586, 422552,
        435700, 449033, 462560, 476285, 490216,
        504360, 518724, 533316, 548144, 563216,
        578543, 594134, 609999, 626149, 642595,
        659352, 676431, 693847, 711615, 729752,
        748275, 767202, 786555, 806356, 826627,
        847395, 868688, 890537, 912974, 936037,
        959766, 984206
    };

    my_barometer->pressurePa = my_barometer->pressureMbar *10;

    if (my_barometer->pressurePa > PA_INIT)
        my_barometer->heightCm = lookupTable[0];
    else {
        const int32_t inx = (PA_INIT - my_barometer->pressurePa) >> 10;
        if (inx >= PZLUT_ENTRIES - 1)
            my_barometer->heightCm = lookupTable[PZLUT_ENTRIES - 1];
        else {
            const int32_t pa1 = PA_INIT - (inx << 10);
            const int32_t z1 = lookupTable[inx];
            const int32_t z2 = lookupTable[inx + 1];
            my_barometer->heightCm = (z1 + (((pa1 - my_barometer->pressurePa) * (z2 - z1)) >> 10));
        }
    }
}

*/

void readMS5803Coefficients() {

    u8_t dummy_tx = 0xFF;
    u8_t dummy_rx;

    for (u8_t coeff_num = 0; coeff_num < 6 ; ++coeff_num ) {
        delay_us(600);
        u8_t _cmd = MS5803_CMD_PROM_READ + ((coeff_num+1)*2);
        delay_us(600);
        pinLow(cs_baro);
//      dummy_rx = spiDataTransfer(SPI1, _cmd);
//      u8_t _byte1 = spiDataTransfer(SPI1,dummy_tx);
//      u8_t _byte2 = spiDataTransfer(SPI1,dummy_tx);
        byteOut(spi2SCK,spi2MOSI, _cmd);
        u8_t _byte1 = byteIn(spi2SCK,spi2MISO);
        u8_t _byte2 = byteIn(spi2SCK,spi2MISO);
        pinHigh(cs_baro);
        coefficients_[coeff_num] = (_byte1 << 8) | _byte2;
    }

}

void initMS5803Barometer() {

    u8_t dummy_tx = 0xFF;
    u8_t dummy_rx;

    pinLow(cs_baro);
//   dummy_rx = spiDataTransfer(SPI1, MS5803_CMD_RES);
    byteOut(spi2SCK,spi2MOSI, MS5803_CMD_RES);
    pinHigh(cs_baro);
    delay_ms(200);
    readMS5803Coefficients();

    //my_barometer->heightCm = 4000;
    //my_barometer->pressurePa = 10000;
    //my_barometer->rawPressureData = 4311550;
    //my_barometer->rawTempatureData = 8387300;
    //my_barometer->temperatureCelcus = 2000;




}
