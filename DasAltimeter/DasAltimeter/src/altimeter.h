

typedef struct Barometer {

    uint16_t coefficients_[6];

    uint32_t rawPressureData;

    uint32_t rawTempatureDate;

    uint32_t temperatureCelcus;

    uint32_t pressurePa;

    uint32_t heightFeet;


} Barometer;


typedef struct 9DofIMU {

    float accelX;

    float accelY;

    float accelZ;

    float gyroX;

    float gyroY;

    float gyroZ;

    float magX;

    float magY;

    float magZ;

    u16_t accelXData;

    u16_t accelYData;

    u16_t accelZData;

    u16_t gyroXData;

    u16_t gyroYData;

    u16_t gyroZData;

    u16_t magXData;

    u16_t magYData;

    u16_t magZData;


} 9DofIMU;


typedef struct AnalogAccelerometer {



} AnalogAccelerometer;


typedef struct Altimeter {

    Barometer myBarometer;

    9DofIMU my9DofIMU;

    AnalogAccelerometer myAnalogAccelerometer;

} Altimeter;