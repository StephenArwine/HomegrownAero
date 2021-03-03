

#include "util.h"



bool gclk_enabled(uint8_t gclk);
void disable_gclk(uint8_t gclk);


void connect_gclk_to_peripheral(uint8_t gclk, uint8_t peripheral);
void disconnect_gclk_from_peripheral(uint8_t gclk, uint8_t peripheral);

void enable_clock_generator(uint8_t gclk, uint32_t source, uint16_t divisor);
void disable_clock_generator(uint8_t gclk);

void clock_init(bool has_crystal, uint32_t dfll48m_fine_calibration);
void init_dynamic_clocks(void);




