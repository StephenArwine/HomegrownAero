#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <saml10e16a.h>

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;

typedef uint8_t SercomId;

typedef struct sample_t {

    uint32_t sampleCount;

    double pressureMbar;

    uint32_t temperatureCelcus;

    double pressureAltitude;

    double lastSampleAltitude;

    uint32_t lastSampleTime;

    uint32_t sampleTime;

    uint32_t dT;

    double dX;

    double dxdt;

    double AVGdXdT;

} sample_t;






bool buzzing;
bool quiteTime;
bool takeSample;
sample_t sample;


#define SPI1 1


//my includes
#include <port.h>
#include <MS5803.h>


const static Pin BuzzerPin = {.group = 0, .pin = 5};

const static Pin spi1MOSI = {.group = 0, .pin = 16, .mux = MUX_PA16C_SERCOM1_PAD0};
const static Pin spi1MISO = {.group = 0, .pin = 17, .mux = MUX_PA17C_SERCOM1_PAD1};
const static Pin spi1SCK = {.group = 0, .pin = 19, .mux = MUX_PA19C_SERCOM1_PAD3};
const static Pin cs_baro = {.group = 0, .pin = 18};

const static Pin USART0TX = {.group = 0, .pin = 24, .mux = MUX_PA24C_SERCOM0_PAD2};
const static Pin USART0RX = {.group = 0, .pin = 22, .mux = MUX_PA22C_SERCOM0_PAD0};
const static Pin USART2TX = {.group = 0, .pin = 14, .mux = MUX_PA14C_SERCOM2_PAD2};
const static Pin USART2RX = {.group = 0, .pin = 15, .mux = MUX_PA15C_SERCOM2_PAD3};
const static Pin GPSINT = {.group = 0, .pin = 11};





//sample.c
void fillBuffers();

//buzzer.c
void beep(u16_t duration);
void beepms(u16_t dur);

//clocks.c
void GclkInit(uint8_t clk_system);
void delayInit(void);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);
uint32_t millis(void);
void rtcInit();
void RTCdelay(u16_t);
void TC0Init();
void TC1Init();
void TC2Init();
void setBuzzerFreq(u32_t freq);

//SercomSPI.c
void SPIinit(u8_t id);
u8_t spiDataTransfer(SercomId id, u8_t data);
void spiDataOut(SercomId id, u8_t data);
u8_t spiDataIn(SercomId id);

//SercomUSART.c
void USART0init();
void USART2init();
void usartDataOut(SercomId id, u8_t data);
u8_t usartDataIn(SercomId id);

//SAMM8Q.c
void parseGPSMessage();



#define ATOMIC_SECTION_ENTER   { register uint32_t __atomic; \
	__asm volatile ("mrs %0, primask" : "=r" (__atomic) ); \
	__asm volatile ("cpsid i");
#define ATOMIC_SECTION_LEAVE   __asm volatile ("msr primask, %0" : : "r" (__atomic) ); }


static inline void delay_cycles(const uint32_t n) {
    if (n > 0) {
        SysTick->LOAD = n;
        SysTick->VAL = 0;
        SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {};

        SysTick->CTRL = 0;

    }
}

inline static Sercom* sercom(SercomId id) {
    return (Sercom*) (0x42000400U + id * 1024);
}