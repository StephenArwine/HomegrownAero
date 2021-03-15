#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <samd51j19a.h>


typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;

typedef uint8_t SercomId;

#define SPI0 0
#define SPI1 1
#define SPI2 2
#define RN4871 2
#define SPI3 3
#define SPI4 4
#define SPI5 5
//#define SPI6
//#define SPI7





typedef struct sample_t {

    uint32_t tempRaw;

    u32_t pressureRaw;

    bool takeSample;

    uint32_t sampleCount;

    double pressureMbar;

    uint32_t temperatureCelcus;

    float pressureAltitude;
	

    double lastSampleAltitude;

    uint32_t lastSampleTime;

    uint32_t sampleTime;

    uint32_t dT;

    double dX;

    double dxdt;

    double AVGdXdT;

    u8_t ADXLX0;

    u8_t ADXLX1;

    u8_t ADXLY0;

    u8_t ADXLY1;

    u8_t ADXLZ0;

    u8_t ADXLZ1;


    int16_t Xaccel;

    int16_t Yaccel;

    int16_t Zaccel;

    float Xfloat;

    float Yfloat;

    float Zfloat;

} sample_t;

sample_t sample;



typedef struct buzzer {

    bool buzzing;

    bool buzzingNeedsStarted;

    bool buzzingScheduled;

    u32_t buzzingUntil;

    u32_t startBuzzingAt;


} buzzer_t;

buzzer_t buzzer;



//my includes
#include <port.h>
#include <clocks.h>
#include <MS5803.h>
#include <adxl345.h>



const static Pin BuzzerPin = {.group = 1, .pin = 22};
const static Pin LedPin = {.group = 1, .pin = 16};
const static Pin PeriphEN = {.group = 1, .pin = 12};
const static Pin RstLck = {.group = 1, .pin = 23};




const static Pin spi0MOSI = {.group = 0, .pin = 8, .mux = MUX_PA08C_SERCOM0_PAD0};
const static Pin spi0MISO = {.group = 0, .pin = 10, .mux = MUX_PA10C_SERCOM0_PAD2};
const static Pin spi0SCK = {.group = 0, .pin = 9, .mux = MUX_PA09C_SERCOM0_PAD1};
const static Pin cs_baro = {.group = 0, .pin = 7};
const static Pin cs_accel = {.group = 0, .pin = 11};

const static Pin spi5MOSI = {.group = 1, .pin = 2, .mux = MUX_PB02D_SERCOM5_PAD0};
const static Pin spi5MISO = {.group = 1, .pin =1, .mux = MUX_PB01D_SERCOM5_PAD3};
const static Pin spi5SCK = {.group = 1, .pin = 3, .mux = MUX_PB03D_SERCOM5_PAD1};
const static Pin cs_mem = {.group = 1, .pin = 0};

const static Pin USART2TX = {.group = 0, .pin = 12, .mux = MUX_PA12C_SERCOM2_PAD0};
const static Pin USART2RX = {.group = 0, .pin = 13, .mux = MUX_PA13C_SERCOM2_PAD1};
const static Pin USART3TX = {.group = 0, .pin = 22, .mux = MUX_PA22C_SERCOM3_PAD0};
const static Pin USART3RX = {.group = 0, .pin = 23, .mux = MUX_PA23C_SERCOM3_PAD1};

const static Pin RN4870_NRST = {.group = 0, .pin = 14};
const static Pin RN4870_MODE = {.group = 0, .pin = 15};


// const static Pin GPSInt = {.group = 0, .pin = 00};


static inline void enablePerif() {
    pinHigh(PeriphEN);
}

static inline void disablePerif() {
    pinLow(PeriphEN);
}

#define ATOMIC_SECTION_ENTER   { register uint32_t __atomic; \
	__asm volatile ("mrs %0, primask" : "=r" (__atomic) ); \
	__asm volatile ("cpsid i");
#define ATOMIC_SECTION_LEAVE   __asm volatile ("msr primask, %0" : : "r" (__atomic) ); }


//util.c
void init();
void POST();

//buzzer.c
void buzzerTick(u32_t time);

//sample.c
void getBaroData();
void cookAccelData();
void getAccelData();
void getSample();


//clocks.c
void delayInit(void);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);
void TC0Init();
void TC2Init();
void TC1Init();


//ADXL345.c
u8_t adxlReadByte(u8_t address);
void adxlWriteByte(u8_t address, u8_t data);
void ADXLgetAccelData();

//sercomSPI.c
void SPI0init(u8_t id);
void SPI5init(u8_t id);
u8_t spiDataTransfer(SercomId id, u8_t data);
void spiDataOut(SercomId id, u8_t data);
u8_t spiDataIn(SercomId id);

//sercomUSART.c
void USART2init();
void USART3init();
void usartDataOut(SercomId id, u8_t data);
u8_t usartDataIn(SercomId id);

//RN4871.c
u8_t RN4870CMDMODE();
u8_t RN4871Status();
void RN4871SetName();


inline static Sercom* sercom(SercomId id) {
    if ( id == 0) {
        return (Sercom*) (0x40003000U);
    } else if ( id == 1) {
        return (Sercom*) (0x40003400U);
    } else if ( id == 2) {
        return (Sercom*) (0x41012000U);
    } else if ( id == 3) {
        return (Sercom*) (0x41014000U);
    } else if ( id == 4) {
        return (Sercom*) (0x43000000U);
    } else if ( id == 5) {
        return (Sercom*) (0x43000400U);
    }

}
