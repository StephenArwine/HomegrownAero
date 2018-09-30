#include <util.h>

bool SampleBool;
bool takeSample();

void pullSample();
void sampleTaken();

void sampleTick();

typedef struct voltages {

    u16_t batV;

    float batFloat;

    u16_t senseA;

    u16_t senseB;

    u16_t senseC;

    u16_t senseD;

} voltages;

typedef struct sample_t {

    u16_t tickDelta;

    u32_t sampleTick;

    voltages voltage;

    u16_t analogRaw;

    float analogAccel;

    int16_t accelXint;
    float accelX;
    int16_t accelYint;
    float accelY;
    int16_t accelZint;
    float accelZ;

    int16_t gyroXint;
    float gyroX;

    int16_t gyroYint;
    float gyroY;

    int16_t gyroZint;
    float gyroZ;

    int32_t pressureMbar;

    uint32_t temperatureCelcus;

    double pressureAltitude;



} sample_t;

extern sample_t sample;

int16_t twosComp(u8_t byteOne, u8_t byteTwo);