
#include <MS5803.h>
#include <boardDefines.h>

void readMS5803Coefficients(Barometer myBarometer) {
	
	
for (uint8_t coeff_num = 0; coeff_num < 6 ; ++coeff_num ) {
	delay_us(600);
	uint8_t _cmd = MS5803_CMD_PROM_READ + ((coeff_num+1)*2);
	delay_us(600);
	pinLow(cs_baro);
	byteOut(spi2SCK,spi2MOSI,_cmd);
	volatile uint8_t _byte1 = byteIn(spi2SCK,spi2MISO);
	volatile uint8_t _byte2 = byteIn(spi2SCK,spi2MISO);
	pinHigh(cs_baro);
	myBarometer.coefficients_[coeff_num] = (_byte1 << 8) | _byte2;
}



}
