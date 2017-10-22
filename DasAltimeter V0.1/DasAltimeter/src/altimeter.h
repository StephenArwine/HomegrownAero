




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

    float magX;

    float magY;

    float magZ;

    u8_t accelXHigh;
    u8_t accelXLow;

    u8_t accelYHigh;
    u8_t accelYLow;

    u8_t accelZHigh;
    u8_t accelZLow;

    u16_t gyroXData;

    u16_t gyroYData;

    u16_t gyroZData;

    u16_t magXData;

    u16_t magYData;

    u16_t magZData;


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


} Altimeter;
