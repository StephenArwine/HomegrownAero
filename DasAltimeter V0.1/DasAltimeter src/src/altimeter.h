
#ifndef ALTIMETER_H_
#define ALTIMETER_H_





void IMUinit();



typedef struct Barometer {

    u16_t coefficients_[6];

    uint32_t rawPressureData;

    uint32_t rawTempatureData;

    uint32_t temperatureCelcus;

    uint32_t pressurePa;

    uint32_t pressureMbar;

    float heightCm;

    float heightFeet;


} Barometer;



typedef struct IMU {
	
	u32_t offsetBufferTime;
	
	float gravityOffset;
	
	float gravityOffsetBuffer;

    float accelX;
	
	float perviousAccelX;

    float accelY;

    float accelZ;

    float gyroX;

    float gyroY;

    float gyroZ;

    u8_t accelXHigh;
    u8_t accelXLow;
    int16_t accelXint;

    u8_t accelYHigh;
    u8_t accelYLow;
    int16_t accelYint;

    u8_t accelZHigh;
    u8_t accelZLow;
    int16_t accelZint;

    u8_t gyroXHigh;
    u8_t gyroXLow;
    int16_t gyroXint;

    u8_t gyroYHigh;
    u8_t gyroYLow;
    int16_t gyroYint;

    u8_t gyroZHigh;
    u8_t gyroZLow;
    int16_t gyroZint;



} IMU;


typedef struct AnalogAccelerometer {

    u16_t analogRaw;

    float analogAccel;



} AnalogAccelerometer;

enum flightState {

    flightStatrup = 0,
    flightIdle = 1,
    flightPad = 2,
    flightBoost = 3,
    flightFast = 4,
    flightCoast = 5,
    flightDrogue = 6,
    flightMain = 7,
    flightLanded = 8,
    flightTest = 10

} flightState;


typedef struct Altimeter {

    enum flightState myFlightState;

    uint32_t currentAddress;

    Barometer myBarometer;

    IMU myIMU;

    AnalogAccelerometer myAnalogAccelerometer;

    float batV;

    u16_t senseA;

    u16_t senseB;

    u16_t senseC;

    u16_t senseD;
	
	u32_t sampleTick;
	
	u32_t lastSampleTick;


} Altimeter;


#endif