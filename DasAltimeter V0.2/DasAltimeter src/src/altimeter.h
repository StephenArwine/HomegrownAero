
#ifndef ALTIMETER_H_
#define ALTIMETER_H_





void IMUinit();


typedef struct deploymentCharges {

    bool Afired;
    bool Bfired;
    bool Cfired;
    bool Dfired;

} deploymentCharges;

extern deploymentCharges charges;
deploymentCharges charges;


typedef struct offsets_t {


    u32_t offsetBufferTime;

    double gravityOffset;

    double gravityOffsetBuffer;

    uint32_t groundOffset;

    uint32_t groundOffsetBuffer;

    uint32_t groundTemperature;

    uint32_t groundTemperatureBuffer;

    u32_t StartupTick;

    u32_t tickDelta;


} offsets_t;

extern offsets_t offsets;
offsets_t offsets;



typedef enum flightState_t {

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

} flightState_t;

extern flightState_t flightState;


typedef struct Altimeter {

    u32_t maxAltitude;

    u8_t flightNumb;

    u32_t flightTime;


} Altimeter;

extern Altimeter altimeter;
Altimeter altimeter;

extern u32_t startupTick;
u32_t startupTick;

bool USARTconnectionAvaliable();
void sendTheasePagesToComputer(u32_t firstPage, u32_t lastPage);
int32_t altitudeAGL();
bool unplugged();

void finishFlight();

void POST();

#endif

