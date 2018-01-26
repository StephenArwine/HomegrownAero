#include <MS5803.h>
#include <boardDefines.h>
#include <math.h>
//#include <tgmath.h>


u32_t readMS5803AdcResults() {

    u8_t dummy_tx = 0xFF;


    pinLow(cs_baro);
    spiDataTransfer(SPI2, cmdAdcRead_);
    u8_t _byte1 = spiDataTransfer(SPI2,dummy_tx);
    u8_t _byte2 = spiDataTransfer(SPI2,dummy_tx);
    u8_t _byte3 = spiDataTransfer(SPI2,dummy_tx);
    pinHigh(cs_baro);

    u32_t _receive = (_byte1 << 16) | (_byte2 << 8) | (_byte3);
    return _receive;
}

void ConvertPressureTemperature(u32_t pressureRaw, u32_t tempRaw, u32_t* temperatureCelcus, int32_t* pressureMbar ) {
    const u16_t C1 = coefficients_[1];
    const u16_t C2 = coefficients_[2];
    const u16_t C3 = coefficients_[3];
    const u16_t C4 = coefficients_[4];
    const u16_t C5 = coefficients_[5];
    const u16_t C6 = coefficients_[6];

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



void readMS5803Coefficients() {

    u8_t dummy_tx = 0xFF;

    for (u8_t coeff_num = 1; coeff_num < 7 ; ++coeff_num ) {
        delay_us(600);
        u8_t _cmd = MS5803_CMD_PROM_READ + ((coeff_num)*2);
        delay_us(600);
        pinLow(cs_baro);
        spiDataTransfer(SPI2, _cmd);
        u8_t _byte1 = spiDataTransfer(SPI2,dummy_tx);
        u8_t _byte2 = spiDataTransfer(SPI2,dummy_tx);
        pinHigh(cs_baro);
        coefficients_[coeff_num] = (_byte1 << 8) | _byte2;
    }

}

void initMS5803Barometer() {


    pinLow(cs_baro);
    spiDataTransfer(SPI2, MS5803_CMD_RES);

    pinHigh(cs_baro);
    delay_ms(200);
    readMS5803Coefficients();

}

u8_t MS5803_CRC4() {

    u8_t count;
    u8_t n_rem;
    u8_t crc_read;
    u8_t n_bit;

    n_rem = 0x00;
    crc_read = coefficients_[7];
    coefficients_[7] = (0xFF00 & (coefficients_[7]));

    for (count = 0; count < 16; count++) {
        if (count%2 == 1) {
            n_rem ^= (unsigned short) ((coefficients_[count>>1]) & 0x00FF);
        } else {
            n_rem ^= (unsigned short) (coefficients_[count>>1]>>8);
        }
        for (n_bit = 8; n_bit > 0; n_bit--) {
            if(n_rem & (0x8000)) {
                n_rem = (n_rem << 1) ^ 0x3000;
            } else {
                n_rem = (n_rem << 1);
            }
        }
    }
    n_rem = (0x000F & (n_rem >> 12));
    coefficients_[7] = crc_read;
    return (n_rem & 0x00);
}
