
#ifndef ALTIMETER_H_
#define ALTIMETER_H_





void IMUinit();



typedef struct Barometer {

    u16_t coefficients_[6];

    uint32_t rawPressureData;

    uint32_t rawTempatureData;

    uint32_t temperatureCelcus;

    uint32_t pressurePa;

    int32_t pressureMbar;

    uint32_t heightCm;

    u32_t heightFeet;

    double altitudefeet;

    uint32_t groundOffset;

    uint32_t groundOffsetBuffer;

    uint32_t groundTemperature;

    uint32_t groundTemperatureBuffer;


} Barometer;



typedef struct IMU {

    u32_t offsetBufferTime;

    double gravityOffset;

    double gravityOffsetBuffer;

    float accelX;

    float perviousAccelX;

    float accelY;

    float accelZ;

    float gyroX;

    float gyroY;

    float gyroZ;

    u16_t accelXRaw;
    int16_t accelXint;

    u16_t accelYRaw;
    int16_t accelYint;

    u16_t accelZRaw;
    int16_t accelZint;

    u16_t gyroXRaw;
    int16_t gyroXint;

    u16_t gyroYRaw;
    int16_t gyroYint;

    u16_t gyroZRaw;
    int16_t gyroZint;

} IMU;

typedef struct flashMemory {

    uint32_t currentAddress;

    uint32_t endingAddress;

    u8_t pageToWrite[256];

    u8_t pageLocation;

    u8_t pageBuffer[256];

    bool pageReady;

} flashMemory;

typedef struct kalmanFilter {

    double  kgain[3][2];
    double est[3];
    double estp[3];

    int16_t kalmanAccel;
    uint32_t kalmanAltitude;

} kalmanFilter;


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

    u8_t flightNumb;

    Barometer myBarometer;

    kalmanFilter myKalmanFilter;

    IMU myIMU;

    AnalogAccelerometer myAnalogAccelerometer;

    flashMemory myFlashMemory;

    u16_t batV;

    float batFloat;

    u16_t senseA;

    u16_t senseB;

    u16_t senseC;

    u16_t senseD;

    u32_t sampleTick;

    u32_t tickDelta;


} Altimeter;


bool USARTconnectionAvaliable();
void sendTheasePagesToComputer(u32_t firstPage, u32_t lastPage);

#endif

