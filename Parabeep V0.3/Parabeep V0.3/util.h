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
bool buzzing;

#define SPI0 0
#define SPI5 5

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

sample_t sample;



//my includes
#include <port.h>
#include <clocks.h>
#include <MS5803.h>
#include <adxl345.h>



const static Pin BuzzerPin = {.group = 0, .pin = 20};
const static Pin LedPin = {.group = 1, .pin = 16};
const static Pin PeriphEN = {.group = 1, .pin = 12};

const static Pin spi0MOSI = {.group = 0, .pin = 8, .mux = MUX_PA08C_SERCOM0_PAD0};
const static Pin spi0MISO = {.group = 0, .pin = 10, .mux = MUX_PA10C_SERCOM0_PAD2};
const static Pin spi0SCK = {.group = 0, .pin = 9, .mux = MUX_PA09C_SERCOM0_PAD1};
const static Pin cs_baro = {.group = 0, .pin = 7};
const static Pin cs_accel = {.group = 0, .pin = 11};

const static Pin spi5MOSI = {.group = 1, .pin = 2, .mux = MUX_PB02D_SERCOM5_PAD0};
const static Pin spi5MISO = {.group = 1, .pin =1, .mux = MUX_PB01D_SERCOM5_PAD3};
const static Pin spi5SCK = {.group = 1, .pin = 3, .mux = MUX_PB03D_SERCOM5_PAD1};
const static Pin cs_mem = {.group = 1, .pin = 0};






//clocks.c
void delayInit(void);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);
void TC0Init();
void TC2Init();

//sercomSPI.c
void SPI0init(u8_t id);
void SPI5init(u8_t id);
u8_t spiDataTransfer(SercomId id, u8_t data);
void spiDataOut(SercomId id, u8_t data);
u8_t spiDataIn(SercomId id);

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
