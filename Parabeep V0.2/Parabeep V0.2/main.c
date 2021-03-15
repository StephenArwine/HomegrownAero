/*
 * Parabeep V0.2.c
 *
 * Created: 2/6/2021 7:59:13 AM
 * Author : Desktop
 */


#include "sam.h"
#include "util.h"




int main(void) {

    init();
    POST();

    pinHigh(RN4870_NRST);


    u32_t tick = millis();
    u32_t lastTime = tick;
    volatile u8_t message[255];
    volatile u8_t message2[255];
    u8_t message2Length = 0;
    u8_t messageLength = 0;

    u8_t catchReboot = RN4871Status();
    RN4871SetName();


    //u8_t catchcmd = RN4871Status();


    while (1) {
        tick = millis();

        buzzerTick(tick);

//         if(sercom(3)->USART.INTFLAG.bit.RXC == 1) {
//             u8_t digit = usartDataIn(3);
//             usartDataOut(RN4871, digit);
//         }
// 
//         if(sercom(RN4871)->USART.INTFLAG.bit.RXC == 1) {
//             u8_t digit = usartDataIn(RN4871);
//             usartDataOut(3, digit);
//         }


        if (sample.takeSample) {
            sample.takeSample = false;
            getSample();
        }



        if ((tick - lastTime) > 500) {
            char buffer[50];
			
            RN4871SendLK8EX1();


            lastTime = tick;
            pinToggle(LedPin);

        }
    }
}