 #pragma once 
 
  #include <samd21g18a.h>
  #include <samd21.h>
 #include <stdbool.h> 
 #include <stdint.h> 


 typedef uint8_t u8_t; 
 typedef uint16_t u16_t; 
 typedef uint32_t u32_t; 
 
 #define NULL 0
 
 void GclkInit();
 
 
 typedef struct Pin {  
      uint8_t  mux;  
      uint8_t  cfg;  
      uint8_t  group;  
      uint8_t  pin;  
      uint8_t  chan;  
  } Pin;  


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