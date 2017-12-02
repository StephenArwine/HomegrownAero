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


