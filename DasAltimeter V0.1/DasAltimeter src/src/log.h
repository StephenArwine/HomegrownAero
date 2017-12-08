#ifndef LOG_H_
#define LOG_H_



#include <util.h>


#define FLIGHT_LOG 'F'
#define SENSOR_LOG 'A'
#define VOLTAGE_TEMP_LOG 'V'
#define STATE_LOG 'S'


extern u32_t tickSample;
extern u8_t flightNumSample;
extern u32_t pressureSample;
extern u32_t tempatureSample;
extern u16_t accelXSample;
extern u16_t accelYSample;
extern u16_t accelZSample;
extern u16_t gyroXSample;
extern u16_t gyroYSample;
extern u16_t gyroZSample;
extern u16_t analogAccelSample;
extern u16_t batVoltageSample;
extern u16_t senseASample;
extern u16_t senseBSample;
extern u16_t senseCSample;
extern u16_t senseDSample;
extern char stateSample;
extern u8_t reasonCodeSample;


struct dataLog {
    char logType;					/* 0  */
    u8_t cSum;						/* 1  */
    uint32_t tick;					/* 2  */
    union {
        /* FLIGHT_LOG  */
        struct {
            u8_t flightNum;			/* 6   */
            u32_t groundPress;		/* 7   */
            u32_t groundTemp;		/* 11  */
            u16_t groundAccelX;		/* 15  */
            u16_t groundAccelY;		/* 17  */
            u16_t groundAccelZ;		/* 19  */
        } flightL;					/* 21 total  */
        /* SENSOR_LOG */
        struct {
            u32_t press;			/* 6   */
            u16_t accelX;			/* 10  */
            u16_t accelY;			/* 12  */
            u16_t accelZ;			/* 14  */
            u16_t gyroX;			/* 16  */
            u16_t gyroY;			/* 18  */
            u16_t gyroZ;			/* 20  */
            u16_t analogAccel;		/* 22  */
        } sensorL;					/* 24 total */
        /* STATE_LOG */
        struct {
            char state;				/* 6  */
            u8_t reasonCode;		/* 7 */
        } state;					/* 8 total */
        /* VOLTAGE_TEMP_LOG */
        struct {
            u16_t batteryVoltage;	/* 6   */
            u16_t senseA;			/* 8   */
            u16_t senseB;			/* 10  */
            u16_t senceC;			/* 12  */
            u16_t senceD;			/* 14  */
            u32_t temp;				/* 16  */
        } vAndT;					/* 20 total */
    } u;
};




void makePage(Altimeter *my_altimeter, u8_t bytesToWrite, u8_t *data);
void logSensors(Altimeter *my_altimeter);
void logFlight(Altimeter *my_altimeter);


u32_t getFlightStartAddress(u8_t flightToFind);
bool isFlightLogged( u8_t flightNumbToCheck); 
u32_t findNextBlankPage(u8_t lastFlightStart);
void findNewFlightStart(Altimeter *my_altimeter);  



#endif
