#ifndef LOG_H_
#define LOG_H_



#include <util.h>


#define FLIGHT_LOG 'F'
#define SENSOR_LOG 'S'
#define EVENT_LOG 'E'
#define VOLTAGE_TEMP_LOG 'V'
#define STATE_LOG 'S'
#define SENSOR_LOG_LENGTH 35

extern uint32_t currentAddress;

extern uint32_t endingAddress;

extern u8_t pageToWrite[256];

extern u8_t pageLocation;


u8_t pageBuffer[256];

extern bool pageReady;

u8_t flightNumb;



void makePage(u8_t bytesToWrite, u8_t* data);
void writePadBuffer(u8_t bytesToWrite, u8_t* data);
void logSensors();
void logSensorsOnPad();
void logEvent(u8_t eventType);
void logFlight();
void writeGroundLog();

void writeFlightStartAddress();
void beginFlightLog();

void writeFlightEndAddress();
u32_t getFlightStartAddress(u8_t flightToFind);
bool isFlightLogged(u8_t flightNumbToCheck);
u32_t findNextBlankPage(u8_t lastFlightStart);
u32_t FindFlightEndingAddress(u8_t findThisFlightsEnd);
void findNewFlightStart();



#endif
