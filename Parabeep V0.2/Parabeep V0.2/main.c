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

    u8_t catchReboot = RN4871Status();
    RN4871SetName();

    TC1_enable_interupt(); //bluetooth

    warmSensors(10);


    while (1) {
        tick = millis();
        buzzerTick(tick);


        if (sample.takeSample) {
            sample.takeSample = false;
            getSample();
            vario();

        }
        if (sample.sendBluetoothPacket) {
            sample.sendBluetoothPacket = false;

            RN4871SendLK8EX1();
            sendDebugData();
        }



        if ((tick - lastTime) > 1000) {
            lastTime = tick;
            pinToggle(LedPin);

        }

        if (tick > 10000) {
            EnterSleepModeOFF();
        }
    }
}