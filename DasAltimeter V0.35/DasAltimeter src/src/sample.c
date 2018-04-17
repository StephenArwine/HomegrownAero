#include <util.h>
#include <boardDefines.h>
#include <MS5803.h>

bool takeSample() {
    return SampleBool;
}

void pullSample() {
    SampleBool = true;
}

void sampleTaken() {
    SampleBool = false;
}

sample_t sample;

void sampleTick() {
    SampleBool = false;

    // take last samples time to discover Dt before setting new time
    u32_t lastTick = sample.sampleTick;
    sample.sampleTick = millis() - startupTick;
    sample.tickDelta = sample.sampleTick - lastTick;

    // altimeter.flightTime = sample.sampleTick - offsets.StartupTick;



    sample.voltage.batV = adc_read(senseBatPin);

    sample.voltage.batFloat = sample.voltage.batV * 0.00259;

    sample.voltage.senseA = adc_read(senseAPin);
    sample.voltage.senseB = adc_read(senseBPin);
    sample.voltage.senseC = adc_read(senseCPin);
    sample.voltage.senseD = adc_read(senseDPin);


    ADC->CTRLB.reg |= ADC_CTRLB_RESSEL_16BIT;
    ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_16;

    sample.analogRaw = adc_read(analogAccelPin);
    sample.analogAccel = (sample.analogRaw - 48695) * 0.00487;

    ADC->AVGCTRL.reg = ADC_AVGCTRL_SAMPLENUM_1;
    ADC->CTRLB.reg |= ADC_CTRLB_RESSEL_12BIT;

    pinLow(cs_baro);
    spiDataOut(BARO_SPI, 0x50);
    pinHigh(cs_baro);
    delay_us(600);
    uint32_t tempRaw = readMS5803AdcResults();

    pinLow(cs_baro);
    spiDataOut(BARO_SPI, 0x40);
    pinHigh(cs_baro);

    u8_t byteOne;
    u8_t byteTwo;
// Accel data
    pinLow(cs_accel);
    spiDataOut(ACCEL_SPI, BMI055_X_ACC_LSB | BMI055_READ_REG);
    byteOne = spiDataIn(ACCEL_SPI);
    byteTwo = spiDataIn(ACCEL_SPI);
    sample.accelXint = twosComp(byteOne,byteTwo);
    byteOne = spiDataIn(ACCEL_SPI);
    byteTwo = spiDataIn(ACCEL_SPI);
    sample.accelYint = twosComp(byteOne,byteTwo);
    byteOne = spiDataIn(ACCEL_SPI);
    byteTwo = spiDataIn(ACCEL_SPI);
    sample.accelZint = twosComp(byteOne,byteTwo);
    pinHigh(cs_accel);

    sample.accelXint =  sample.accelXint >> 4;
    sample.accelYint =  sample.accelYint >> 4;
    sample.accelZint =  sample.accelZint >> 4;

    sample.accelX = sample.accelXint * BMI055_ACCEL_16G_DIV;
    sample.accelY = sample.accelYint * BMI055_ACCEL_16G_DIV;
	
    if (altimeter.pointingUp) {
        sample.accelZ = -sample.accelZint * BMI055_ACCEL_16G_DIV;
    } else {
        sample.accelZ = sample.accelZint * BMI055_ACCEL_16G_DIV;
    }

    // Gyro data
    pinLow(cs_gyro);
    spiDataOut(GYRO_SPI, BMI055_X_GYRO_LSB | BMI055_READ_REG);
    byteOne = spiDataIn(GYRO_SPI);
    byteTwo = spiDataIn(GYRO_SPI);
    sample.gyroXint = twosComp(byteOne,byteTwo);
    byteOne = spiDataIn(GYRO_SPI);
    byteTwo = spiDataIn(GYRO_SPI);
    sample.gyroYint = twosComp(byteOne,byteTwo);
    byteOne = spiDataIn(GYRO_SPI);
    byteTwo = spiDataIn(GYRO_SPI);
    sample.gyroZint = twosComp(byteOne,byteTwo);
    pinHigh(cs_gyro);

    sample.gyroXint = sample.gyroXint >> 4;
    sample.gyroYint = sample.gyroYint >> 4;
    sample.gyroZint = sample.gyroZint >> 4;

    sample.gyroX =  sample.gyroXint * BMI055_GYRO_2000DS_DIV;
    sample.gyroY =  sample.gyroYint * BMI055_GYRO_2000DS_DIV;
    sample.gyroZ =  sample.gyroZint * BMI055_GYRO_2000DS_DIV;


    delay_us(400);
    u32_t PressureRaw = readMS5803AdcResults();
    ConvertPressureTemperature(PressureRaw, tempRaw, &sample.temperatureCelcus, &sample.pressureMbar);
    sample.pressureAltitude = paToFeetNOAA(sample.pressureMbar);
}

int16_t twosComp(u8_t byteOne, u8_t byteTwo) {

    u16_t comp = byteOne | (byteTwo << 8);

    if ((byteTwo >> 7) == 1) { // negative
        comp =  comp | ~((1 << 16) - 1);

    } else { // positive
// 		nothing else needed, comp done
    }
    return comp;
}
