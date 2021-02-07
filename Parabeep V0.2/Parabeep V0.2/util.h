#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <samd51g19a.h>


typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;

typedef uint8_t SercomId;

//my includes
#include <port.h>


const static Pin BuzzerPin = {.group = 0, .pin = 5};
const static Pin LedPin = {.group = 1, .pin = 11};

//clocks.c
void delayInit(void);
void delay_ms(uint32_t delay);
void delay_us(uint32_t delay);

//sercomSPI.c
void SPIinit(u8_t id);

inline static Sercom* sercom(SercomId id) {
	return (Sercom*) (0x40003000U + id * 400);
}