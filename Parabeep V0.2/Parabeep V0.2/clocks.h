

#include "util.h"

void delayInit(void);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);
void TC0Init();
void TC1Init();
void TC2Init();


bool gclk_enabled(uint8_t gclk);
void disable_gclk(uint8_t gclk);


void connect_gclk_to_peripheral(uint8_t gclk, uint8_t peripheral);
void disconnect_gclk_from_peripheral(uint8_t gclk, uint8_t peripheral);

void enable_clock_generator(uint8_t gclk, uint32_t source, uint16_t divisor);
void disable_clock_generator(uint8_t gclk);

void clock_init(bool has_crystal, uint32_t dfll48m_fine_calibration);
void init_dynamic_clocks(void);

void init_clock_source_osculp32k(void);

void rtcInit();
void RTC_Handler(void);
void TC0_Handler(void);
void TC1_Handler(void);
void TC2_Handler(void);

void TC0_enable_interupt();
void TC1_enable_interupt();
void TC2_enable_interupt();

void TC0_disable_interupt();
void TC1_disable_interupt();
void TC2_disable_interupt();

void TC2_setDur(u8_t dur);


uint32_t millis(void);

