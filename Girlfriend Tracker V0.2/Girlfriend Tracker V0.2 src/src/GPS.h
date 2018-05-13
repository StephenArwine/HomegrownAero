


#include <util.h>

typedef struct messageToSend {

    bool transmitMessage;
    bool messageReady;
    uint32_t tick;
    u32_t hits;
    char time[9];
    char lat[10];
    char northSouth[1];
    char longitude[11];
    char eastWest[1];
    char quality[1];
    char numSV[2];
    char HDOP[4];
    char altitude[10];
    char speedOverGround[10];
    char courseOverGround[10];


} messageToSend;

extern messageToSend myMessage;
messageToSend myMessage;

u8_t findNextComma(char *message);
void sendUSARTMessage(messageToSend myMessage);
void parseGPSMessage();