#pragma once

#include <samd21g18a.h>
#include <samd21.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;


// my includes
#include <port.h>

void GclkInit();

#define CONTAINER_OF(ptr, type, field_name) \
 ((type *) (((uint8_t *)ptr) - offsetof(type, field_name)))

#define ATOMIC_SECTION_ENTER   { register uint32_t __atomic; \
	 __asm volatile ("mrs %0, primask" : "=r" (__atomic) ); \
	 __asm volatile ("cpsid i");
#define ATOMIC_SECTION_LEAVE   __asm volatile ("msr primask, %0" : : "r" (__atomic) ); }

void delayInit(void);

static inline void delay_cycles(
    const uint32_t n) {
    if (n > 0) {
        SysTick->LOAD = n;
        SysTick->VAL = 0;

        while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) {
        };
    }
}

/**
 * \def delay_ms
 * \brief Delay in at least specified number of milliseconds.
 * \param delay Delay in milliseconds
 */
void delay_ms(uint32_t delay);

/**
 * \def delay_us
 * \brief Delay in at least specified number of microseconds.
 * \param delay Delay in microseconds
 */
void delay_us(uint32_t delay);
