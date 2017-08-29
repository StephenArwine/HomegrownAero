#include <util.h>


// GPS stuff for MTK3329 Module
#ifdef HAS_MTK3329
 struct MTK3329Module {
    float timef;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    float longitude;
    float latitude;
	bool validSalad;
};

#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define PMTK_SET_BAUD_57600 "$PMTK251,57600*2C"
#define PMTK_SET_BAUD_9600 "$PMTK251,9600*17"
#define PMTK_LOCUS_STARTLOG  "$PMTK185,0*22"
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23"

void MTK3329ParseGPRMC(struct MTK3329Module *const myMTK3329,uint8_t *const RMCMessage);
void MTK3329ParseMessage(struct MTK3329Module *const myMTK3329,uint8_t *const messageToParse);

#endif

// Flash mem stuff for MS5607
#ifdef HAS_AT25SF

#define OPCODE_SLOWREAD_ARRAY 0x03
#define OPCODE_WRITEENABLE    0x06
#define OPCODE_PROGRAM        0x02
#define OPCODE_04K_ERASE      0x20  // Erase a  4K block
#define OPCODE_32K_ERASE      0x52

static const uint32_t memoryBlock34KHeaders[] = {0x000000, 0x008000, 0x010000, 0x018000, 0x020000, 0x028000, 0x030000, 0x038000, 0x040000, 0x048000, 0x050000, 0x058000, 0x060000, 0x068000, 0x070000, 0x078000, 0x080000, 0x087000, 0x090000, 0x098000, 0x0A0000, 0x0A8000, 0x0B0000, 0x0B8000, 0x0C0000, 0x0C8000, 0x0D0000, 0x0D8000, 0x0E0000, 0x0E8000, 0x0F0000, 0x0F8000};


uint8_t AT25SFGetByte(uint32_t address);
void AT25SFWriteByte(uint32_t address, uint8_t byteToWrite);
void AT25SFErace32KBlock(uint8_t blockToErace);
void AT25SFErace4KBlock(uint8_t blockToErace);


#endif
