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
    sample.tickDelta = millis() - sample.sampleTick;
    sample.sampleTick = millis();
    altimeter.flightTime = sample.sampleTick - offsets.StartupTick;

    sample.voltage.batV = adc_read(senseBatPin);
    sample.voltage.batFloat = sample.voltage.batV * 0.0019;

    sample.voltage.senseA = adc_read(senseAPin);
    sample.voltage.senseB = adc_read(senseBPin);
    sample.voltage.senseC = adc_read(senseCPin);
    sample.voltage.senseD = adc_read(senseDPin);

    sample.analogRaw = adc_read(analogAccelPin);
    sample.analogAccel = (sample.analogRaw - 3900) * -0.0154;



    uint8_t dummy_Tx = 0xFF;
    uint8_t dummy_rx;

    pinLow(cs_baro);
    dummy_rx = spiDataTransfer(SPI2, 0x50);
    pinHigh(cs_baro);
    delay_us(600);
    uint32_t tempRaw = readMS5803AdcResults();

    pinLow(cs_baro);
    dummy_rx = spiDataTransfer(SPI2, 0x40);
    pinHigh(cs_baro);


    u8_t byteOne;
    u8_t byteTwo;
// Accel data
    pinLow(cs_accel);
    dummy_rx = spiDataTransfer(SPI0, BMI055_X_ACC_LSB | BMI055_READ_REG);
    byteOne = spiDataTransfer(SPI0,dummy_Tx);
    byteTwo = spiDataTransfer(SPI0,dummy_Tx);
    int16_t accelXint = twosComp(byteOne,byteTwo);
    byteOne = spiDataTransfer(SPI0,dummy_Tx);
    byteTwo = spiDataTransfer(SPI0,dummy_Tx);
    int16_t accelYint = twosComp(byteOne,byteTwo);
    byteOne = spiDataTransfer(SPI0,dummy_Tx);
    byteTwo = spiDataTransfer(SPI0,dummy_Tx);
    int16_t accelZint = twosComp(byteOne,byteTwo);
    pinHigh(cs_accel);

    accelXint =  accelXint >> 4;
    accelYint =  accelYint >> 4;
    accelZint =  accelZint >> 4;

    sample.accelX =  accelXint * BMI055_ACCEL_16G_DIV;
    sample.accelY =  accelYint * BMI055_ACCEL_16G_DIV;
    sample.accelZ = -accelZint * BMI055_ACCEL_16G_DIV;


    // Gyro data
    pinLow(cs_gyro);
    dummy_rx = spiDataTransfer(SPI0, BMI055_X_GYRO_LSB | BMI055_READ_REG);
    byteOne = spiDataTransfer(SPI0,dummy_Tx);
    byteTwo = spiDataTransfer(SPI0,dummy_Tx);
    int16_t gyroXint = twosComp(byteOne,byteTwo);
    byteOne = spiDataTransfer(SPI0,dummy_Tx);
    byteTwo = spiDataTransfer(SPI0,dummy_Tx);
    int16_t gyroYint = twosComp(byteOne,byteTwo);
    byteOne = spiDataTransfer(SPI0,dummy_Tx);
    byteTwo = spiDataTransfer(SPI0,dummy_Tx);
    int16_t gyroZint = twosComp(byteOne,byteTwo);
    pinHigh(cs_gyro);

    gyroXint = gyroXint >> 4;
    gyroYint = gyroYint >> 4;
    gyroZint = gyroZint >> 4;

    sample.gyroX =  gyroXint * BMI055_GYRO_2000DS_DIV;
    sample.gyroY =  gyroYint * BMI055_GYRO_2000DS_DIV;
    sample.gyroZ =  gyroZint * BMI055_GYRO_2000DS_DIV;


    delay_us(400);
    u32_t PressureRaw = readMS5803AdcResults();
    ConvertPressureTemperature(PressureRaw, tempRaw, &sample.temperatureCelcus, &sample.pressureMbar);
    sample.altitudefeet = paToFeetNOAA(sample.pressureMbar);

    if ( (PressureRaw < 10) | (tempRaw < 10)) {
        beep(400);
    }

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
