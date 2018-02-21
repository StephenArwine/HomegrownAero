



#include <util.h>

#define MS5803_CMD_RES    0x1E
#define cmdAdcRead_  0x00
#define cmdAdcD1_    0x40
#define cmdAdcD2_    0x50
#define cmdAdcD1_4096_    0x48
#define cmdAdcD2_4096_    0x58
#define MS5803_CMD_PROM_READ  0xA0

u16_t coefficients_[8];

u32_t readMS5803AdcResults();

void ConvertPressureTemperature(u32_t pressureRaw, u32_t tempRaw, u32_t* temperatureCelcus, int32_t* pressureMbar);
double paToFeetNOAA(int32_t pressureMbar);


void readMS5803Coefficients();
void initMS5803Barometer();
//void pascalToCent(Barometer *my_barometer);
u8_t MS5803_CRC4();

