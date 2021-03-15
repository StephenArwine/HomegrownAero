#include <util.h>


void getBaroData() {
    pinLow(cs_baro);
    spiDataOut(SPI0, cmdAdcD2_4096_);
    pinHigh(cs_baro);

    delay_us(9040);
    sample.tempRaw = readMS5803AdcResults();

    pinLow(cs_baro);
    spiDataOut(SPI0, cmdAdcD1_4096_);
    pinHigh(cs_baro);

    delay_us(9040);
    sample.pressureRaw = readMS5803AdcResults();

}

void cookAccelData() {

    int16_t negative = (sample.ADXLX1 & (1 <<7)) != 0;
    if (negative) {
        // if negative then preform 2's complement to int conversion
        sample.Xaccel = ((sample.ADXLX1 | ~((1 << 8) - 1)) << 8 ) |  sample.ADXLX0;
    } else {
        sample.Xaccel = (sample.ADXLX1 << 8) +  sample.ADXLX0;
    }
    negative = (sample.ADXLY1 & (1 <<7)) != 0;
    if (negative) {
        sample.Yaccel = ((sample.ADXLY1 | ~((1 << 8) - 1)) << 8 ) | sample.ADXLY0;
    } else {
        sample.Yaccel = (sample.ADXLY1 << 8) | sample.ADXLY0;
    }
    negative = (sample.ADXLZ1 & (1 <<7)) != 0;
    if (negative) {
        sample.Zaccel = ((sample.ADXLZ1 | ~((1 << 8) - 1)) << 8 ) | sample.ADXLZ0;
    } else {
        sample.Zaccel = (sample.ADXLZ1 << 8) | sample.ADXLZ0;
    }

    sample.Xfloat = sample.Xaccel * ADXL345_MG2G_MULTIPLIER;
    sample.Yfloat = sample.Yaccel * ADXL345_MG2G_MULTIPLIER;
    sample.Zfloat = sample.Zaccel * ADXL345_MG2G_MULTIPLIER;
}

void getAccelData() {
    ADXLgetAccelData();
    cookAccelData();
}

void getSample() {

    getBaroData();
    getAccelData();

    ConvertPressureTemperature(sample.pressureRaw, sample.tempRaw, &sample.temperatureCelcus, &sample.pressureMbar);
    sample.pressureAltitude = paToFeetNOAA(sample.pressureMbar);
	
    ADXLgetAccelData();

    sample.lastSampleTime = sample.sampleTime;
    sample.sampleTime = millis();
    sample.dT = sample.sampleTime - sample.lastSampleTime;
    sample.sampleCount = sample.sampleCount + 1;
}

