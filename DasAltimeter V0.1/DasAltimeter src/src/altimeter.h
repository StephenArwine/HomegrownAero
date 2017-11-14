
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

    float accelX;

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


typedef struct Altimeter {

    Barometer myBarometer;

    IMU myIMU;

    AnalogAccelerometer myAnalogAccelerometer;

    float batV;

    u16_t senseA;

    u16_t senseB;

    u16_t senseC;

    u16_t senseD;


} Altimeter;
