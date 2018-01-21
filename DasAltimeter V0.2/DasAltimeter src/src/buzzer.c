#include <util.h>
#include <boardDefines.h>


void beep(u16_t toggles) {

    pinToggle(LedPin);
    for (u16_t buzz = 0; buzz < toggles; ++buzz) {
        pinToggle(buzzerPin);
        delay_us(122);
    }
    pinLow(buzzerPin);
    pinToggle(LedPin);
}

void startupJingle(){
    beep(300);
    delay_ms(80);
    beep(300);
    delay_ms(250);
    beep(300);
    delay_ms(80);
    beep(300);
    delay_ms(500);
}

void unpluggedJingle(){
            delay_ms(80);
            beep(300);
            delay_ms(80);
            beep(300);
            delay_ms(80);
            beep(300);
            delay_ms(80);
            pinLow(buzzerPin);
            pinLow(LedPin);

}


