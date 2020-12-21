/*
 * DasAltimeter V0.35.c
 *
 * Created: 4/21/2018 6:34:21 PM
 * Author : ancar
 */


#include "sam.h"

/*** SysTick ***/

volatile uint32_t g_msTicks;

/* SysTick IRQ handler */
void SysTick_Handler(void) {
    g_msTicks++;
}

void delay_ms(unsigned ms) {
    unsigned start = g_msTicks;
    while (g_msTicks - start <= ms) {
        __WFI();
    }
}

void init_systick() {

    if (SysTick_Config(48000000 / 1000)) {	/* Setup SysTick Timer for 1 msec interrupts  */
        while (1) {

        }								/* Capture error */
    }
    NVIC_SetPriority(SysTick_IRQn, 0x0);
    g_msTicks = 0;

}


int main(void) {
    /* Initialize the SAM system */
    SystemInit();
    init_systick();
    system_clocks_init();

    volatile uint32_t x = 0;

    /* Replace with your application code */
    while (1) {
        delay_ms(10);
        x++;
    }
}
