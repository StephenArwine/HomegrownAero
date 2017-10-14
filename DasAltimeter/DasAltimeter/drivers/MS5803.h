



#include <util.h>

#define MS5803_CMD_RES    0x1E
#define cmdAdcRead_  0x00
#define cmdAdcD1_    0x40
#define cmdAdcD2_    0x50
#define cmdAdcD1_4096_    0x48
#define cmdAdcD2_4096_    0x58
#define MS5803_CMD_PROM_READ  0xA0

u32_t readMS5803AdcResults();

void ConvertPressureTemperature(Barometer *my_barometer);
void paToFeetNOAA(Barometer *my_barometer);


void readMS5803Coefficients(Barometer *my_barometer);
void initMS5803Barometer(Barometer *my_barometer);
void pascalToCent(Barometer *my_barometer);

